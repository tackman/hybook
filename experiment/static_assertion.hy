(print "define macro")

(defn f1 [n]
  (print "f1" n)
  n)

(defn f2 [n]
  (print "f2" n)
  (* n 2))

(defn f3 [n]
  (print "f3" n)
  (* n 3))

(defmacro f1* [n]
  (, `(f1 ~n) n))

(defmacro f2* [n]
  (assert (= n 100))
  (, `(f2 ~n) (* n 2)))

(defmacro f3* [n]
  (assert (= n 1500))
  (, `(f3 ~n) (* n 3)))

(f1* 10)
(f2* 100)
(f3* 1500)
