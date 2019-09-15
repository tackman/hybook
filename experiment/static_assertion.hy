(print "runtime start")

(defn f1 [n]
  (print "f1" n)
  n)

(defn f2 [n]
  (print "f2" n)
  (* n 2))

(defmacro f1* [n]
  (, `(f1 ~n) n))

(defmacro f2* [t]
  (print "f2*" t)
  (print (second t))
  (assert (= 100 (second t)))
  (, `(f2 (first ~t)) (* (second t) 2)))


(f2* (f1* 100))

(-> (f1* 100)
    (f2*))

; Assertion error!
; (-> (f1* 54331)
;    (f2*))
