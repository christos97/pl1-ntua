(* https://stackoverflow.com/questions/3918288/turning-a-string-into-a-char-list-list-using-sml *)
fun linelist file =
    let 
        (* A function to read an integer from specified input. *)
        fun readInt input = 
        	Option.valOf (TextIO.scanStream (Int.scan StringCvt.DEC) input)

        val inStream = TextIO.openIn file

        val n = readInt inStream
        val m = readInt inStream

        val _ = TextIO.inputLine inStream
        val row   = TextIO.inputAll inStream
        val _ = TextIO.closeIn inStream
    in 
        (n, m, String.tokens Char.isSpace row)
    end;

fun showMatrixInt arr =
    Array2.appi Array2.RowMajor (fn (_, col, c) =>
      print (Int.toString c ^ (if col + 1 = Array2.nCols arr then "\n" else " " )))
      {base=arr,row=0,col=0,nrows=NONE,ncols=NONE}

fun showMatrixStr arr =
    Array2.appi Array2.RowMajor (fn (_, col, c) =>
      print (str c ^ (if col + 1 = Array2.nCols arr then "\n" else " " )))
      {base=arr,row=0,col=0,nrows=NONE,ncols=NONE}

fun fixTable board i j value= 
    let 
        val k = Array2.update(board, i, j, value)
    in
        board
    end;

(*
0   INIT
~1  OUT_OF_BOARDERS
~2  BLACK_LIST
3   UP
4   DOWN
5   LEFT
6   RIGHT
*)
fun first ~1 _ n m graph board winnable = (board, winnable)
|   first i j n m graph board winnable = 
    let
        val perimeter = (i = 0) orelse (i = n-1) orelse (j = 0) orelse (j = m-1)
        val (ni, nj) = if (j = 0) then (i-1, m-1) else (i, j-1)
    in
        case (Array2.sub(graph, i, j), perimeter)
        of  (#"U", true) => 
                if (i = 0) then first ni nj n m graph (fixTable board i j ~1) ((i, j) :: winnable)
                else first ni nj n m graph (fixTable board i j 3) winnable
        |   (#"D", true) => 
                if (i = n-1) then first ni nj n m graph (fixTable board i j ~1) ((i, j) :: winnable)
                else first ni nj n m graph (fixTable board i j 4) winnable
        |   (#"L", true) => 
                if (j = 0) then first ni nj n m graph (fixTable board i j ~1) ((i, j) :: winnable)
                else first ni nj n m graph (fixTable board i j 5) winnable
        |   (#"R", true) => 
                if (j = m-1) then first ni nj n m graph (fixTable board i j ~1) ((i, j) :: winnable)
                else first ni nj n m graph (fixTable board i j 6) winnable
        |   (#"U", false) => first ni nj n m graph (fixTable board i j 3) winnable
        |   (#"D", false) => first ni nj n m graph (fixTable board i j 4) winnable
        |   (#"L", false) => first ni nj n m graph (fixTable board i j 5) winnable
        |   (#"R", false) => first ni nj n m graph (fixTable board i j 6) winnable
        |   _ => first ~1 ~1 n m graph board []
    end

fun printl(~1, graph : (int * int) list) = print "\n"
|	printl(i, graph) = 
    let
        val graphI = List.nth(graph,i)
        val k = print("(" ^ Int.toString(#1 graphI) ^ "," ^ Int.toString(#2 graphI) ^ "), ")
                
    in 
        printl(i-1, graph)
    end;

fun changeAll board value [] = board
|   changeAll board value ((x, y)::xs) = 
    let
        val k = Array2.update(board, x, y, value)
    in
        changeAll board value xs
    end;

fun second ~1 _ n m (board, winnable, deadly) = deadly
|   second i j n m (board, winnable, deadly) = 
    let
        val (ni, nj) = if (j = 0) then (i-1, m-1) else (i, j-1)

        fun innerLoop ~1 ~1 board winnable deadly _ = (board, winnable, deadly)
        |   innerLoop i j board winnable deadly visited = 
            if (Array2.sub(board, i, j) = ~1) then 
                innerLoop ~1 ~1 (changeAll board ~1 (tl visited)) ((tl visited) @ winnable) deadly []
            else if (Array2.sub(board, i, j) = ~2) then 
                innerLoop ~1 ~1 (changeAll board ~2 (tl visited)) winnable ((tl visited) @ deadly) []
            else
                if List.exists (fn x => x = (i, j)) (tl visited) then 
                    innerLoop ~1 ~1 (changeAll board ~2 (tl visited)) winnable ((tl visited) @ deadly) []
                else 
                    let
                        val p = Array2.sub(board, i, j)
                        val (nI, nJ) = if (p = 3) then (i-1, j)
                                else if (p = 4) then (i+1, j)
                                else if (p = 5) then (i, j-1) 
                                else (i, j+1)
                    in
                        innerLoop nI nJ board winnable deadly ((nI, nJ) :: visited)
                    end 
    in
        second ni nj n m (innerLoop i j board winnable deadly [(i, j)])
    end;

fun loop_rooms file = 
    let
        val (n, m, temp) = linelist file
        val graph = Array2.fromList (map String.explode temp)
        val b = Array2.array(n, m, 0)
        val (board, winnable) = first (n-1) (m-1) n m graph b []
        val final = length (second (n-1) (m-1) n m (board, winnable, []))
    in
        print (Int.toString final ^ "\n")
    end;