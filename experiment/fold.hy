(defn foldl [v xs op]
  (for [x xs]
    (setv v (op v x)))
  v)


; sample
; (foldl "AAA" ["hoge" "fuga"] +)
