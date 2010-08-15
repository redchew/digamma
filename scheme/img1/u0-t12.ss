(load "./t12.ss")
(def img (make-vector (* 300 600) 0))
;(draw-y-axis img 150 0 600)
;(draw-x-axis img 0 300 300 600)
;(draw-x-axis img 0 300 300 600)
(display (length img))
(newline)
(plot-fixed-line img 10 10 300 290 300 600)
(plot-fixed-line img 1 1 300 600 300 600)
(plot-fixed-line img 0 600 300 0 300 600)
(plot-fixed-rectangle img 120 120 50 50 300 600)
(plot-fixed-rectangle img 50 50 10 20 300 600)
(def fil (open "u0.pnm" :write))
(display (format "P1~%300 600~%~a" img) fil)
(close fil)