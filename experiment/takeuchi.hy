
(defn tarai [x y z]
  (if (<= x y) y
      (tarai (tarai (- x 1) y z) (tarai (- y 1) z x) (tarai (- z 1) x y))))

(import time)

(setv start (time.perf_counter))
(print "begin takeuchi benchmark...")
(print (tarai 14 6 0))

(print "take time " (- (time.perf_counter) start))

