type Point = {xCoord: float; yCoord:float}

let point1 = 
    {xCoord = 0.1; yCoord = 0.1}

let point2 =
    {xCoord = 0.8; yCoord = 0.8}

let makePoint x y =
    {xCoord = x; yCoord = y}

let r = System.Random()

let randomPoint () =
    let x = r.NextDouble()
    let y = r.NextDouble()
    makePoint x y

let throwDarts n =
    List.init n (fun n -> randomPoint())

type HitResult = 
    | Hit
    | Miss of float

let isHit point =
    let hitValue = sqrt (pown point.xCoord 2 + pown point.yCoord 2)
    let didHit = hitValue <= 1.0
    match didHit with
    | true -> Hit
    | false -> Miss (hitValue - 1.0)

let countHits n =
    n
    |> throwDarts
    |> List.map isHit
    |> List.filter (fun x -> x = Hit)
    |> List.length
    // This should be a reduce in here.

let estimatePi n =
    float (countHits n) / float n * 4.0


[<EntryPoint>]
let main argv = 
    printfn "%A" argv
    0 // return an integer exit code
