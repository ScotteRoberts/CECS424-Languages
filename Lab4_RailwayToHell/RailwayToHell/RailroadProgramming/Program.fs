open System

// Suppose I have a website where a user is attempting to register an account. I can 
// encapsulate such a request as a record:

type RegistrationAttempt = {username: string; email: string}

// As in:
let reg1 = {username = "neal"; email = "neal.terrell@csulb.edu"}

// Suppose I want to write a function to validate a registration attempt, checking against
// many rules for new accounts. In an OOP language, it might look something like this:

// I can then build a single monolithic "validate" function:
let validateRegistration reg =
    let {username = name; email = em} = reg
    if String.length name = 0 then
        "Missing a username"
    elif not (em.Contains("@")) then
        "Invalid email address"
    else
        "Valid"

(*
This solution has many problems:
1. The monolithic approach is difficult to maintain in general. As the number of validation 
    requirements grows, the if/elif chain grows to an unmaintainable size.
2. The return type (string) is unideal. We want to return a message if the validation fails,
    or some kind of True value if it succeeds. "Valid" as a string is an ugly hack, and the type
    system can't do us any favors here.
3. In a larger system, we might also want to do things like normalize the email address
    or otherwise transform the input when validation succeeds. The return type again hinders us.

A better design would incorporate a better return type that could discriminate between success
and failure, and would break the the validation requirements into individual functions.
Let's try it!
*)


// A better return type:
type ValidationResult =
    | Success of RegistrationAttempt
    | Failure of string

// A function can now return a ValidationResult, indicating success or failure. We can also
// start composing our validation functions.
let usernameExists reg =
    if reg.username.Length > 0 then
        Success reg
    else
        Failure "Username must not be blank"
// username: RegistrationAttempt -> ValidationResult

let emailHasAtSign reg = 
    if reg.email.Contains("@") then
        Success reg
    else
        Failure "Email address must contain a @"

let emailHasLocalPart reg =
    if reg.email.IndexOf("@") > 0 then
        Success reg
    else
        Failure "Email address does not have a local-part"

// Ideally, we would compose these functions such that failing the first validation
// means we don't attempt any later validations. Picture this like a railroad "switch":
// one track is the Success track, leading from one validation function to the next, as
// long as the validations are successful. The other track is the Failure track, which
// a validation function can send us to if the validation fails, and once we are on the 
// Failure track, we can never get back to the Success track.

// F# programmers call this pattern "Railway oriented programming": viewing the composition
// of functions as having more than one "track", with each function being responsible
// for moving to the correct track based on input.

// Picture time!

// Problem: we would love to compose our two validation functions, so one flows to the 
// next. But F# complains about the following line. Why?

// let validate2 = usernameExists >> emailHasAtSign

// usernameExists outputs a RegistrationResult, but emailHasAtSign expects a RegistrationAttempt,
// because emailHasAtSign is on the "Success" track and doesn't want to be bothered by
// failed attempts. In railway terminology, we call these "switches" -- they take a single
// track input and spit out a two-track output, which means we can't connect the output
// of one to the input of another... without some help.

let connectValidations reg =
    match usernameExists reg with
    | Success s -> emailHasAtSign s
    | Failure f -> Failure f

// Try it!
connectValidations {username = ""; email="a@b.com"} |> printfn "%A"
connectValidations {username = "asdf"; email="b.com"} |> printfn "%A"
connectValidations reg1 |> printfn "%A"

// But this is too specific; we want something more generic. In railway terms, we want
// a means of converting an existing switch function to a two-track input, which could then
// accept the two-track output of another function. We call this concept "bind": given a switch
// function and a two-track input, only call the switch function if the input is on the Success 
// track; otherwise bypass the switch function.

let bind switchFunction twoTrackInput =
    match twoTrackInput with
    | Success s -> switchFunction s
    | Failure f -> Failure f

// Now, we have a more generic way of connecting switch functions
let validate2 =
    usernameExists
    >> bind emailHasAtSign
    >> bind emailHasLocalPart

validate2 reg1 |> printfn "\n\nvalidate2 with bind:\n%A"


// Binding is so common that we can introduce an operator to make our life easier.
let (>=>) switch1 switch2 reg =
    bind switch2 (switch1 reg)

// And now we get
let validate3 =
    usernameExists
    >=> emailHasAtSign
    >=> emailHasLocalPart

// **************************************************** End of Lecture Code **********************************


// Reference for further documentation: https://fsharpforfunandprofit.com/posts/recipe-part2/


// Question 1/2: Create lookup tables to test validation methods.
let existingAccounts = ["scott@yahoo.com"; "friend@gmail.com"; "james@juice.gov"; "glob@glerb.glerb"; "jaxthedestroyerofworlds@kittens.com"]
let blacklistedDomains = ["mailinator.org"; "throwawaymail.com"]


(*
 Question 3:
 Write a new validation (switch) function uniqueEmail, which takes a RegistrationAttempt and validates 
 that the email address is not in the existingAccounts list.
*)
let uniqueEmail reg =
    if not (List.contains(reg.email) existingAccounts) then
        Success reg
    else
        Failure "Email is already used"


(*
 Question 4:
 Write a new validation function emailNotBlacklisted, which takes a RegistrationAttempt and validates 
 that the domain of the email address (following the @) is not in the blacklistedDomains list.
*)
let getEmailDomain reg =
    reg.email.IndexOf('@') + 1  // + 1 can cause out of bounds errors. Fix sometime.
    |> reg.email.Substring        

let getEmailLocalPart reg =
    reg.email.Substring(0, reg.email.IndexOf('@'))

let emailNotBlacklisted reg =
    let domain = getEmailDomain reg
    if not (List.contains(domain) blacklistedDomains) then
        Success reg
    else
        Failure "This email's domain is blacklisted"


// Question 5: Single Track functions

(*
 Part A:
 Write a function switch, which promotes a single-track function to a switch function. switch takes 
 a function f and a RegistrationAttempt as parameters, invokes the function on the attempt, and 
 returns Success with the resulting value. (Thereby converting f to a switch function, of one input -> 
 two outputs.)

 Composition: >=> switch f
*)
let switch f reg = 
    f reg |> Success

(*
Part A (extra):
Sometimes though, you want to use the two-track model directly, in which case you want to turn a one-
track function into a two-track function directly.

Composition: >> map f
*)
let map oneTrackFunction twoTrackInput = 
    match twoTrackInput with
    | Success s -> Success (oneTrackFunction s)
    | Failure f -> Failure f

(*
Part B:
 Write a function lowercaseEmail, which takes a RegistrationAttempt and returns a new RegistrationAttempt
 with the same username and the email address converted to all-lowercase. 
*)
let lowercaseEmail reg =
    {reg with email = reg.email.ToLower()}

(*
Part C:
Write a function canonicalizeEmail, which takes a RegistrationAttempt and canonicalizes the email address
of the attempt by removing all periods and dashes to the left of the @ symbol. The function returns a new 
RegistrationAttempt with the same username as before, and the new canonicalized email.
*)

// Alternative code... Reference: https://stackoverflow.com/questions/20308875/remove-characters-from-string-in-f#20309891
// Usage: (strip ".-" "hello.world-hello") => "helloworldhello"
let strip chars = 
    String.collect (fun c -> if Seq.exists((=)c) chars then "" else string c)

let canonicalizeEmail reg =
    let cleanLocalPart = reg |> getEmailLocalPart |> strip ".-"
    let domain = reg |> getEmailDomain
    {reg with email = cleanLocalPart + "@" + domain}


// Question 6: Incorporate the new functions into the existing validation system:
let validate4 =
    validate3
    >=> switch canonicalizeEmail
    >=> switch lowercaseEmail
    >=> emailNotBlacklisted
    >=> uniqueEmail


// Extra Validation Methods
let emailExists reg =
    if reg.email.Length > 0 then
        Success reg
    else
        Failure "Email must not be blank"


// Final Complete Validation
let validateRegFinal =
    usernameExists
    >=> emailExists
    >=> emailHasAtSign
    >=> emailHasLocalPart
    >=> uniqueEmail
    >=> emailNotBlacklisted
    >=> switch lowercaseEmail
    >=> switch canonicalizeEmail

let validUserNames = ["jim"; "scott"; "Pat"; "GREG"; "BoB"]

(*
Question 7:
Finally, demonstrate your code works by creating enough distinct RegistrationAttempt objects to cover each of
the possible validation failure reasons

Test Possibilities:
1. usernameExists
2. emailExists
3. emailHasAtSign
4. emailHasLocalPart
5. uniqueEmail
6. emailNotBlacklisted
7. lowercaseEmail
8. canonicalizedEmail
*)

let noUsernameNoEmail = {username = ""; email = ""}
let noUsername = {username = ""; email = existingAccounts.Head}
let noEmail = {username = validUserNames.Head; email = ""}
let noEmailAtSign = {username = validUserNames.Head; email = "aaa.com"}
let noEmailLocalPart = {username = validUserNames.Head; email = "@aaa.com"}
let notUniqueEmail = { username = validUserNames.Head; email = existingAccounts.Head}
let blacklistedEmail = { username = validUserNames.Head; email = "scott@" + blacklistedDomains.Head}
let validUppercaseEmail = {username = validUserNames.Head; email = "NEWEMAIL@GMAIL.COM"}
let validNonCanonicalEmail = {username = validUserNames.Head; email = "scoot.roberts@gmail.com"}

let testRegistrationAttempts = [noUsernameNoEmail; noUsername; noEmail; noEmailAtSign; noEmailLocalPart;
                                notUniqueEmail; blacklistedEmail; validUppercaseEmail; validNonCanonicalEmail]

// Test Case Single Execution
let printResult reg =
    validateRegFinal reg |> printfn "Result: %A"

// Test Case Execute All
let printResults regAttempts =
    for i in regAttempts do
        validateRegFinal i |> printfn "Result: %A"
    

[<EntryPoint>]
let main argv = 
    printfn "%A" argv
    0 // return an integer exit code
