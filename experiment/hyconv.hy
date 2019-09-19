(import torch)
(import [torch.nn :as nn])

(defmacro conv2d* [prev Cout kernel-size stride padding dilation]
  (setv (, N Cin Hin Win nets) (eval prev))

  (setv Hout (int (/ (+ Hin (* 2 padding) (- (* dilation (- kernel-size 1))) -1) stride)))
  (setv Wout (int (/ (+ Win (* 2 padding) (- (* dilation (- kernel-size 1))) -1) stride)))


  (,  N Cout Hout Wout `[~nets ``(nn.Conv2d ~~~Cin ~~~Cout ~~~kernel-size ~~~stride ~~~padding ~~~dilation)]))


(defmacro bn2d* [prev]
  (setv (, N Cin Hin Win nets) (eval prev))
;  (print nets)

  (setv num-features (* N Cin Hin Win))

  (, N Cin Hin Win `[~nets ``(nn.BatchNorm2d ~~~num-features)]))


;;; usage example

; following code encounters Hy compiler bug
;(setv net (-> (conv2d* (,  5 3 100 200 `["some" "ary"]) 30 1 1 0 1)
;     (bn2d* )
;     (conv2d* 25 1 1 0 1)
;     (bn2d*)))


(setv net (-> (conv2d* (,  5 3 100 200 `[]) 30 1 1 0 1)
              (bn2d* )
              (conv2d* 35 1 1 0 1))

(print net)
