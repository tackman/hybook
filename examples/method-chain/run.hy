(import A)
(require chain)

(setv a (A.A))

;;; Example method chain with threading macro

(-> (a.f)
    (chain.next g 3)
    (chain.next h 5 10)
    (chain.next f))

