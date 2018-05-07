// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.

// Practice
let coll1 = seq {yield 1; yield 2; yield 3;}
let coll2 = [1;2;3;]
let coll3 = {1 .. 1000}
let tuple1 = (1, 64L, "Boo", false)

let print item =
    printfn "%A" item
let square x = x * x
let add x y = x + y
let subtract x y = x - y

// Homework 5

// Question 4
let list1 = [1; 2; 3; 4; 5; 6; 7; 8]

let listLength _coll =
    _coll
    |> List.length

let listLength2 _coll = List.length _coll

let rec butLast _coll =
    _coll
    |> List.length
    |> function
        | 0 -> []
        | 1 -> []
        | _ -> List.head _coll :: butLast (List.tail _coll)

let countEvens _coll =
    _coll
    |> List.filter (fun el -> el % 2 = 0)
    |> List.length

let intTuples = [(1,1); (1,2); (1,3)]
let sumOddProducts _coll =
    _coll
    |> List.map(fun (x, y) -> x * y)
    |> List.filter(fun x -> x % 2 = 1)
    |> List.sum

// Question 5
type BinaryTree =
    | Empty
    | Node of int * BinaryTree * BinaryTree

let exampleTree = Node (10, 
                       Node (5, 
                            Node (2, Empty, Empty),
                            Node (7, Empty, Empty)
                       ), 
                       Node (15, Empty, Empty)
                  )

let rec findMax tree =
    match tree with
    | Empty              -> System.Int32.MinValue
    | Node (i, _, Empty) -> i
    | Node (_, _, right) -> findMax right

let rec findMin tree =
    match tree with
    | Empty              -> System.Int32.MaxValue
    | Node (i, Empty, _) -> i
    | Node (_, left, _)  -> findMin left

let rec findHeight tree =
    match tree with
    | Empty                 -> -1
    | Node (_, left, right) -> findHeight (max left right) + 1

let rec traverseInOrder tree =
    match tree with
    | Empty                 -> []
    | Node (i, left, right) -> traverseInOrder left @ [i] @ traverseInOrder right

let rec isBST tree =
    match tree with
    | Empty                 -> true
    | Node (i, left, right)


[<EntryPoint>]
let main argv = 
    printfn "%A" argv
    0 // return an integer exit code
