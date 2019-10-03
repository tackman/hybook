(defmacro chain [obj method &rest args]
  `((. ~obj ~method) ~@args))

(defclass A [][x None]
  (defn f [self x]
    (setv self.x x)
    (setv b (B))
    (setv b.x x)
    b))

(defclass B [][x None]
  (defn g [self x y]
    (print "something in B" x y)
    (C)))

(defclass C [][]
  (defn h [self]
    (A)))

(setv a (-> (A)
     (chain f "arg")
     (chain g "fugafuga" "myaaaaa")
     (chain h)
     (chain f "Something result")))

(print a.x)
