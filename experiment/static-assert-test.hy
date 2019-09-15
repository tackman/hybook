(defmacro simple-assert [x]
  (print "compile time" x)
  (assert (= x 100))
  `(print "runtime" ~x))

(setv n 50)
(simple-assert 100)
(simple-assert n)
