(defn third [list]
  (second (next list)))

;; True if the form is a variable (symbol).
(defn variable? [form]
  (symbol? form))

;; True if the two forms are the same variable.
(defn same-variable? [form1 form2]
  (and (variable? form1) (variable? form2) (= form1 form2)))

;; True if the form represents a sum.
(defn sum? [form]
  (and (list? form) (= '+ (first form))))

;; Constructs a sum of a and b.
(defn make-sum [a b]
  (list '+ a b))
  ;(cond
    ;(= a 0) b
    ;(= b 0) a
    ;:else (+ a b)))

;; Selects the addend (first value) of a sum.
(defn addend [sum]
  (second sum))

;; Selects the augend (second value) of a sum.
(defn augend [sum]
  (third sum))

;; True if the form represents a difference.
(defn diff? [form]
  (and (list? form) (= '- (first form))))

;; Constructs a difference of a and b.
(defn make-diff [a b]
  (list '- a b))
  ;(cond
    ;(= a 0) (list '- b)
    ;(= b 0) a
    ;:else (- a b)))

;; Selects the minuend (first value) of a difference.
(defn minuend [diff]
  (second diff))

;; Selects the minuend (second value) of a difference.
(defn subtrahend [diff]
  (third diff))

;; True if the form represents a product.
(defn prod? [form]
  (and (list? form) (= '* (first form))))

;; Constructs a product of a and b.
(defn make-prod [a b]
  (list '* a b))
  ;(cond
    ;(= a 0) 0
    ;(= a 1) b
    ;(= b 0) 0
    ;(= b 1) a
    ;:else (* a b)))

;; Selects the multiplier (first value) of a product.
(defn multiplier [prod]
  (second prod))

;; Selects the multiplicand (second value) of a product.
(defn multiplicand [prod]
  (third prod))

;; True if the form represents a quotient.
(defn quot? [form]
  (and (list? form) (= '/ (first form))))

;; Constructs a quotient of a and b.
(defn make-quot [a b]
  (list '/ a b))
  ;(cond
    ;(= a 0) 0
    ;(= b 1) a
    ;:else (/ a b)))

;; Selets the dividend (top value) of a quotient.
(defn dividend [quotient]
  (second quotient))

;; Selets the divisor (bottom value) of a quotient.
(defn divisor [quotient]
  (third quotient))

;; True if the form represents a power.
(defn power? [form]
  (and (list? form) (= '** (first form))))

;; Constructs a power of a and b.
(defn make-power [a b]
  (list '** a b))
  (cond
    (= b 0) 1
    (= b 1) a
    :else (Math/pow a b)))

;; Selets the base value of the power.
(defn base-power [power]
  (second power))

;; Selets the exponent of the power.
(defn exponent-power [power]
  (third power))

;; True if the form represents a power.
(defn ln? [form]
  (and (list? form) (= 'ln (first form))))

;; Constructs a power of a and b.
(defn log-of [form]
  (second form))

;; Returns the derivative of a function expressed in Clojure notation, where variables are quoted.
;; The second parameter is the variable which the derivative is calculated with respect to.
(defn derivative [form var]
  (cond ; The derivative of a constant is 0
        (number? form) 0
        ; The derivative of a variable is 0 if the variable is not the one being derived; or 1, if it is.
        (variable? form) (if (same-variable? form var) 1 0)
        ; Sum rule
        (sum? form) (make-sum (derivative (addend form) var)
                              (derivative (augend form) var))
        ; Difference rule
        (diff? form) (make-diff (derivative (minuend form) var)
                                (derivative (subtrahend form) var))
        ; Quotient rule
        (quot? form)  (make-quot  (make-diff  (make-prod (divisor form)
                                              (derivative (dividend form) var))
                                              (make-prod  (derivative (divisor form) var)
                                                          (dividend form)))
                                  (make-prod  (divisor form)
                                              (divisor form)))
        ; Power rule
        (power? form) (make-prod (make-prod (exponent-power form) 
                                            (make-power (base-power form) (- (exponent-power form) 1))) 
                                (derivative (base-power form) var))
        ; Natural Log rule
        (ln? form)  (make-quot (derivative (log-of form) var)
                               (log-of form))
        ; Product rule
        (prod? form) (make-sum (make-prod (multiplier form)
                                          (derivative (multiplicand form) var))
                               (make-prod (derivative (multiplier form) var)
                                          (multiplicand form)))))