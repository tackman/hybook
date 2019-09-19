(import torch)
(import [torch.nn :as nn])

(defmacro conv2d* [prev Cout kernel-size stride padding dilation]
  (print prev)
  (setv (, N Cin Hin Win) (eval prev))

  (setv Hout (int (/ (+ Hin (* 2 padding) (- (* dilation (- kernel-size 1))) -1) stride)))
  (setv Wout (int (/ (+ Win (* 2 padding) (- (* dilation (- kernel-size 1))) -1) stride)))


  (setv body `(nn.Conv2d ~Cin ~Cout ~kernel-size ~stride ~padding ~dilation))

  (,  N Cout Hout Wout))


(defmacro bn2d* [prev]
  (print prev)
  (setv (, N Cin Hin Win) (eval prev))
  (setv num-features (* N Cin Hin Win))
  (setv body `(nn.BatchNorm2d ~num-features))

  (, N Cin Hin Win))

;;; usage example

(-> (conv2d* (,  5 3 100 200 ) 30 1 1 0 1)
    (bn2d* )
    (conv2d* 25 1 1 0 1)
    (bn2d*))
