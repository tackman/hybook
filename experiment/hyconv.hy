(import torch)
(import [torch.nn :as nn])

(defmacro 2d [prev Cout kernel-size stride padding dilation]
  (setv (, N Cin Hin Win _) (eval prev))

  (setv Hout `(int (/ (+ ~Hin (* 2 ~padding) (- (* ~dilation (- ~kernel-size 1))) -1) ~stride)))
  (setv Wout `(int (/ (+ ~Win (* 2 ~padding) (- (* ~dilation (- ~kernel-size 1))) -1) ~stride)))

  (setv body `(nn.Conv2d ~Cin ~Cout ~kernel-size ~stride ~padding ~dilation))
  (,  N Cout Hout Wout body))

;;; usage example

(-> (2d (,  5 3 100 200  None) 30 1 1 0 1)
    (2d 25 1 1 0 1))
