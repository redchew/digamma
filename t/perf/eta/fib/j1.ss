; just a simple test to keep around for testing E' with
; you can safely ignore this :D
(def fib (fn (i j n)
	(if (<= n 0)
		i	
		(fib (+ i j) i (- n 1)))))
(def scheme_main (fn ()
    (display (fib 0 1 10))
    (newline)
    (display (fib 0 1 20))
    (newline)
    (display (fib 0 1 30))
    (newline)
    (display (fib 0 1 32))
    (newline)))
(scheme_main)
