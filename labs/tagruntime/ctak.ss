(def (tak x y z)
  (if (> x y)
    (tak
      (tak (- x 1) y z)
      (tak (- y 1) z x)
      (tak (- z 1) x y))
    y))

(def (scheme_main)
    (display (tak 18 12 6))
    (newline))