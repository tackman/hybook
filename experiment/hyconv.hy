(import torch)
(import [torch.nn :as nn])

(defmacro 2d [Cin Hin Win Cout kernel-size stride padding dilation]
  (setv Hout `(int (/ (+ ~Hin (* 2 ~padding) (- (* ~dilation (- ~kernel-size 1))) -1) ~stride)))
  (setv Wout `(int (/ (+ ~Win (* 2 ~padding) (- (* ~dilation (- ~kernel-size 1))) -1) ~stride)))

  (setv body `(nn.Conv2d ~Cin ~Cout ~kernel-size ~stride ~padding ~dilation))
  (, Cout Hout Wout body))

(defmacro hoge [Hin]
  `(int (/ ~Hin 2)))

;;; usage example
;  (setv (, c1 h1 w1 b1) (2d 3 100 200 30 1 1 0 1))
;  (setv (, c2 h2 w2 b2) (2d c1 h1 w1  25 1 1 0 1))
;  (setv (, c3 h3 w3 b3) (2d c2 h2 w2 300 1 1 0 1))
;  (assert (= h3 1500))
