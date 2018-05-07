// Question 1: Create a record type for a Point, which has two float fields
// xCoord and yCoord.
type Point = {xCoord: float; yCoord:float}

// Test Points
let point1 = 
    {xCoord = 0.1; yCoord = 0.1}
let point2 =
    {xCoord = 0.8; yCoord = 0.8}

// Question 2: Implement functionality

// 2a: makePoint, which takes two floats and returns a Point
let makePoint x y =
    {xCoord = x; yCoord = y}

// 2b: randomPoint, which generates two random floats and calls makePoint
let r = System.Random()
let randomPoint () =
    let x = r.NextDouble()
    let y = r.NextDouble()
    makePoint x y

(*
2c: throwDarts, which takes an integer n and generates a list of Point objects of length n. 
Use List.init to create a list of elements; it requires the number of elements to create, 
as well as a function that takes an unused input and returns an object to place into the 
list
*)
let throwDarts n =
    List.init n (fun n -> randomPoint())


// 2di: Declare a descriminated union named HitResult, with two cases: Hit, and Miss of float.
type HitResult = 
    | Hit
    | Miss of float

(*
2dii: Determine if the Point object is a hit, and return Hit if so. Otherwise, determine
the distance from the point to the nearest point on the imaginary circle of redius 1. Return
Miss with that value.
*)
let isHit point =
    let hitValue = sqrt (pown point.xCoord 2 + pown point.yCoord 2)
    let didHit = hitValue <= 1.0
    match didHit with
    | true -> Hit
    | false -> Miss (hitValue - 1.0)

(*
 2e: countHits, which takes an integer for how many darts to throw, throws that many darts, maps
 each throw to a HitResult, and counts how many of those are Hits. 
*)
let countHits n =
    n
    |> throwDarts
    |> List.map isHit
    |> List.filter (fun x -> x = Hit)   // This should be a reduce in here.
    |> List.length
    

(*
2f: estimatePi, which takes an integer for how many darts to throw, and returns a Monte Carlo 
estimate for π.
*)
let estimatePi n =
    float (countHits n) / float n * 4.0


[<EntryPoint>]
let main argv = 
    printfn "%A" argv
    0 // return an integer exit code
