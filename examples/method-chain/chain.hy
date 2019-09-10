(defmacro next [obj method &rest args]
  `((. ~obj ~method) ~@args))
