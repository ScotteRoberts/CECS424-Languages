namespace Homework5

module Practice = 
    let coll1 = seq {yield 1; yield 2; yield 3;}
    let coll2 = [1;2;3;]
    let coll3 = {1 .. 1000}

    let tuple1 = (1, 64L, "Boo", false)

    let print item =
        printfn "%A" item

    let square x = x * x
    let add x y = x + y
    let subtract x y = x - y

module Question4 =
    let list1 = [1; 2; 3]

    let butLast _coll =
        _coll
        |> List.length