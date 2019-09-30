(setv s "Some string")
(setv x [1 2 3])
(setv y [0 4 5])

(setv a (in (in s x) y))
(print a)

; hy2py compiles Traceback most recent call last:
; a = (s in x) in y
