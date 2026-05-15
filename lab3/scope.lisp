(defparameter *x* 5)

(defun foo ()
  (let ((y 10))
    (format t "~a~%" (+ *x* y))))

(defun bar ()
  (let ((*x* 7))
    (foo)))

(defun main ()
  (bar)
  (format t "~a~%" *x*))

(main)