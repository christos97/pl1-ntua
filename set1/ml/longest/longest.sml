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
        fun readInts 0 acc hos sum = acc
          | readInts i acc hos sum = 
          		let
                    val price = sum + ((~1.0 * Real.fromInt(readInt inStream) / Real.fromInt hos) - 1.0)
                in
                    readInts (i - 1) (price :: acc) hos price
                end;
    
    in
   		(days, readInts days [] hospitals 0.0)
    end;

fun max (a : real ,b :real) = if a > b then a else b;
fun min (a,b) = if a < b then a else b;

fun minmax (~1, arr : real list, lmin, rmax, len) = (lmin, rmax)
|	minmax (i, arr, lmin, rmax, len) = minmax (i-1, arr, mim(List.nth(arr, i), hd lmin) :: lmin, max(List.nth(arr, len-i), hd rmax) :: rmax, len);
            
fun loop (min : real list, max : real list, ~1, _, maxdiff) = maxdiff
|	loop (min, max, _, ~1, maxdiff) = maxdiff
|	loop (min, max, i, j, maxdiff) =
    if List.nth(min, i) < List.nth(max, j) then
        if maxdiff > i-j then loop (min, max, i, (j-1), maxdiff)
        else loop (min, max, i, (j-1), (i-j))
    else loop (min, max, (i-1), j, maxdiff);
        
fun maxIndexDiff(arr : real list, n)=
    let
        val (LMin, RMax) = minmax (n-2, arr, List.nth(arr, n-1) :: [], hd arr :: [], n-1)
        val maxDiff = loop (LMin, (rev RMax), (n-1), (n-1), ~1)
    in
        if maxDiff = n-1 then maxDiff+1
        else maxDiff
    end;
    
fun longest fileName =
    let 
        val (days, cases) = parse fileName
        val final = maxIndexDiff (cases, days)
    in
        print (Int.toString final ^ "\n")   
    end;
