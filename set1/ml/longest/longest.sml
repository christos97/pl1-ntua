(* Input parse code by Stavros Aronis, modified by Nick Korasidis. *)
fun parse file =
    let
        (* A function to read an integer from specified input. *)
        fun readInt input = 
        	Option.valOf (TextIO.scanStream (Int.scan StringCvt.DEC) input)

        (* Open input file. *)
        val inStream = TextIO.openIn file

        (* Read an integer (number of countries) and consume newline. *)
        val days = readInt inStream
        val hospitals = readInt inStream
        val _ = TextIO.inputLine inStream

        (* A function to read N integers from the open file. *)
        fun readInts 0 acc hos sum = acc (* Replace with 'rev acc' for proper order. *)
          | readInts i acc hos sum = 
          		let
                    val price = sum + ((~1.0 * Real.fromInt(readInt inStream) / Real.fromInt hos) - 1.0)
                in
                    readInts (i - 1) (price :: acc) hos price
                end;
    
    in
   		(days, readInts days [] hospitals 0.0)
    end;
    
fun fromL (~1, arr : real list, lmin) = lmin
|	fromL (i, arr, lmin) = 
    if List.nth(arr, i) < hd lmin then fromL ((i-1), arr, (List.nth(arr, i) :: lmin)) 
    else fromL ((i-1), arr, ((hd lmin) :: lmin));

fun fromR (~1, arr : real list, rmax) = rmax
|	fromR (i, arr, rmax) = 
    if List.nth(arr, i) > hd rmax then fromR ((i-1), arr, (List.nth(arr, i) :: rmax)) 
    else fromR ((i-1), arr, ((hd rmax) :: rmax));
    
fun loop (min : real list, max : real list, ~1, _, maxdiff) = maxdiff
|	loop (min, max, _, ~1, maxdiff) = maxdiff
|	loop (min, max, i, j, maxdiff) =
    if List.nth(min, i) < List.nth(max, j) then
        if maxdiff > i-j then loop (min, max, i, (j-1), maxdiff)
        else loop (min, max, i, (j-1), (i-j))
    else loop (min, max, (i-1), j, maxdiff);
        
fun maxIndexDiff(arr : real list, n)=
    let
        val LMin = fromL ((n-2), arr, (List.nth(arr, n-1) :: []))
        val k = print ("LMax OK\n")
        val RMax = fromR ((n-2), (rev arr), (hd arr :: []))
        val k = print ("RMax OK\n")
        val maxDiff = loop (LMin, (rev RMax), (n-1), (n-1), ~1)
    in
        if maxDiff = n-1 then maxDiff+1
        else maxDiff
    end;
    
fun longest fileName =
    let 
        val (days, cases) = parse fileName
        val k = print ("Read OK\n")
        
        val final = maxIndexDiff (cases, days)
    in
        print (Int.toString final ^ "\n")   
    end;
