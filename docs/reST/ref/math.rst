.. include:: common.txt

:mod:`pygame.math`
==================

.. autopgmodule:: pygame.math

.. autopgfunction:: clamp

.. autopgfunction:: lerp

.. autopgfunction:: invlerp

.. autopgfunction:: smoothstep

.. autopgfunction:: remap

.. class:: Vector2

   | :sl:`a 2-Dimensional Vector`
   | :sg:`Vector2() -> Vector2(0, 0)`
   | :sg:`Vector2(int) -> Vector2`
   | :sg:`Vector2(float) -> Vector2`
   | :sg:`Vector2(Vector2) -> Vector2`
   | :sg:`Vector2(x, y) -> Vector2`
   | :sg:`Vector2((x, y)) -> Vector2`

   .. versionchanged:: 2.1.3
      Inherited methods of vector subclasses now correctly return an instance of the
      subclass instead of the superclass

   .. method:: dot

      | :sl:`calculates the dot- or scalar-product with the other vector`
      | :sg:`dot(Vector2, /) -> float`

      .. ## Vector2.dot ##

   .. method:: cross

      | :sl:`calculates the cross- or vector-product`
      | :sg:`cross(Vector2, /) -> float`

      calculates the third component of the cross-product.

      .. ## Vector2.cross ##

   .. method:: magnitude

      | :sl:`returns the Euclidean magnitude of the vector.`
      | :sg:`magnitude() -> float`

      calculates the magnitude of the vector which follows from the
      theorem: ``vec.magnitude() == math.sqrt(vec.x**2 + vec.y**2)``

      .. ## Vector2.magnitude ##

   .. method:: magnitude_squared

      | :sl:`returns the squared magnitude of the vector.`
      | :sg:`magnitude_squared() -> float`

      calculates the magnitude of the vector which follows from the
      theorem: ``vec.magnitude_squared() == vec.x**2 + vec.y**2``. This
      is faster than ``vec.magnitude()`` because it avoids the square root.

      .. ## Vector2.magnitude_squared ##

   .. method:: length

      | :sl:`returns the Euclidean length of the vector.`
      | :sg:`length() -> float`

      calculates the Euclidean length of the vector which follows from the
      Pythagorean theorem: ``vec.length() == math.sqrt(vec.x**2 + vec.y**2)``

      .. ## Vector2.length ##

   .. method:: length_squared

      | :sl:`returns the squared Euclidean length of the vector.`
      | :sg:`length_squared() -> float`

      calculates the Euclidean length of the vector which follows from the
      Pythagorean theorem: ``vec.length_squared() == vec.x**2 + vec.y**2``.
      This is faster than ``vec.length()`` because it avoids the square root.

      .. ## Vector2.length_squared ##

   .. method:: normalize

      | :sl:`returns a vector with the same direction but length 1.`
      | :sg:`normalize() -> Vector2`

      Returns a new vector that has ``length`` equal to ``1`` and the same
      direction as self. If the vector is the zero vector (i.e. has length
      ``0`` thus no direction) a ``ValueError`` is raised.

      .. ## Vector2.normalize ##

   .. method:: normalize_ip

      | :sl:`normalizes the vector in place so that its length is 1.`
      | :sg:`normalize_ip() -> None`

      Normalizes the vector so that it has ``length`` equal to ``1``.
      The direction of the vector is not changed. If the vector is the zero
      vector (i.e. has length ``0`` thus no direction) a ``ValueError`` is
      raised.

      .. ## Vector2.normalize_ip ##

   .. method:: is_normalized

      | :sl:`tests if the vector is normalized i.e. has length == 1.`
      | :sg:`is_normalized() -> bool`

      Returns True if the vector has ``length`` equal to ``1``. Otherwise
      it returns ``False``.

      .. ## Vector2.is_normalized ##

   .. method:: scale_to_length

      | :sl:`scales the vector to a given length.`
      | :sg:`scale_to_length(float, /) -> None`

      Scales the vector so that it has the given length. The direction of the
      vector is not changed. You can also scale to length ``0``. If the vector
      is the zero vector (i.e. has length ``0`` thus no direction) a
      ``ValueError`` is raised.

      .. ## Vector2.scale_to_length ##

   .. method:: reflect

      | :sl:`returns a vector reflected of a given normal.`
      | :sg:`reflect(Vector2, /) -> Vector2`

      Returns a new vector that points in the direction as if self would bounce
      of a surface characterized by the given surface normal. The length of the
      new vector is the same as self's.

      .. ## Vector2.reflect ##

   .. method:: reflect_ip

      | :sl:`reflect the vector of a given normal in place.`
      | :sg:`reflect_ip(Vector2, /) -> None`

      Changes the direction of self as if it would have been reflected of a
      surface with the given surface normal.

      .. ## Vector2.reflect_ip ##

   .. method:: distance_to

      | :sl:`calculates the Euclidean distance to a given vector.`
      | :sg:`distance_to(Vector2, /) -> float`

      .. ## Vector2.distance_to ##

   .. method:: distance_squared_to

      | :sl:`calculates the squared Euclidean distance to a given vector.`
      | :sg:`distance_squared_to(Vector2, /) -> float`

      .. ## Vector2.distance_squared_to ##

   .. method:: move_towards

      | :sl:`returns a vector moved toward the target by a given distance.`
      | :sg:`move_towards(Vector2, float, /) -> Vector2`

      Returns a Vector which is moved towards the given Vector by a given
      distance and does not overshoot past its target Vector.
      The first parameter determines the target Vector, while the second
      parameter determines the delta distance. If the distance is in the
      negatives, then it will move away from the target Vector.

      .. versionadded:: 2.1.3

      .. ## Vector2.move_towards ##

   .. method:: move_towards_ip

      | :sl:`moves the vector toward its target at a given distance.`
      | :sg:`move_towards_ip(Vector2, float, /) -> None`

      Moves itself toward the given Vector at a given distance and does not
      overshoot past its target Vector.
      The first parameter determines the target Vector, while the second
      parameter determines the delta distance. If the distance is in the
      negatives, then it will move away from the target Vector.

      .. versionadded:: 2.1.3

      .. ## Vector2.move_towards_ip ##

   .. method:: lerp

      | :sl:`returns a linear interpolation to the given vector.`
      | :sg:`lerp(Vector2, float, /) -> Vector2`

      Returns a Vector which is a linear interpolation between self and the
      given Vector. The second parameter determines how far between self and
      other the result is going to be. It must be a value between ``0`` and
      ``1``, where ``0`` means self and ``1`` means other will be returned.

      .. ## Vector2.lerp ##

   .. method:: slerp

      | :sl:`returns a spherical interpolation to the given vector.`
      | :sg:`slerp(Vector2, float, /) -> Vector2`

      Calculates the spherical interpolation from self to the given Vector. The
      second argument - often called t - must be in the range ``[-1, 1]``. It
      parametrizes where - in between the two vectors - the result should be.
      If a negative value is given the interpolation will not take the
      complement of the shortest path.

      .. ## Vector2.slerp ##

   .. method:: smoothstep

      | :sl:`returns a smooth interpolation to the given vector.`
      | :sg:`smoothstep(Vector2, value, /) -> Vector2`

      Returns a Vector which is a smooth interpolation between self and the
      given Vector. This means that the interpolation follows an s-shaped curve, with
      change happening more slowly near the limits (0.0 and 1.0) and faster in the middle.
      The second parameter determines how far between the two vectors the result is going to be.
      Value is clamped to ``[0, 1]``.

      The formula is:

      ``a * (1 - interp) + b * interp``

      where:

      ``interp = value * value * (3 - 2 * value)``

      .. versionadded:: 2.4.0

      .. ## Vector2.smoothstep ##

   .. method:: elementwise

      | :sl:`The next operation will be performed elementwise.`
      | :sg:`elementwise() -> VectorElementwiseProxy`

      Applies the following operation to each element of the vector.

      .. ## Vector2.elementwise ##

   .. method:: rotate

      | :sl:`rotates a vector by a given angle in degrees.`
      | :sg:`rotate(angle, /) -> Vector2`

      Returns a vector which has the same length as self but is rotated
      counterclockwise by the given angle in degrees.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector2.rotate ##

   .. method:: rotate_rad

      | :sl:`rotates a vector by a given angle in radians.`
      | :sg:`rotate_rad(angle, /) -> Vector2`

      Returns a vector which has the same length as self but is rotated
      counterclockwise by the given angle in radians.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.0.0

      .. ## Vector2.rotate_rad ##

   .. method:: rotate_ip

      | :sl:`rotates the vector by a given angle in degrees in place.`
      | :sg:`rotate_ip(angle, /) -> None`

      Rotates the vector counterclockwise by the given angle in degrees. The
      length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector2.rotate_ip ##

   .. method:: rotate_ip_rad

      | :sl:`rotates the vector by a given angle in radians in place.`
      | :sg:`rotate_ip_rad(angle, /) -> None`

      DEPRECATED: Use rotate_rad_ip() instead.

      .. versionaddedold:: 2.0.0
      .. deprecatedold:: 2.1.1

      .. ## Vector2.rotate_rad_ip ##

   .. method:: rotate_rad_ip

      | :sl:`rotates the vector by a given angle in radians in place.`
      | :sg:`rotate_rad_ip(angle, /) -> None`

      Rotates the vector counterclockwise by the given angle in radians. The
      length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.1.1

      .. ## Vector2.rotate_rad_ip ##

   .. method:: angle_to

      | :sl:`calculates the angle to a given vector in degrees.`
      | :sg:`angle_to(Vector2, /) -> float`

      Returns the angle from self to the passed ``Vector2`` that would rotate self
      to be aligned with the passed ``Vector2`` without crossing over the negative
      x-axis.

      .. figure:: code_examples/angle_to.png
         :alt: angle_to image

         Example demonstrating the angle returned

      .. ## Vector2.angle_to ##

   .. method:: as_polar

      | :sl:`returns a tuple with radial distance and azimuthal angle.`
      | :sg:`as_polar() -> (r, phi)`

      Returns a tuple ``(r, phi)`` where r is the radial distance, and phi
      is the azimuthal angle.

      .. ## Vector2.as_polar ##

   .. method:: from_polar

      | :sl:`Sets x and y from a polar coordinates tuple.`
      | :sg:`from_polar((r, phi), /) -> None`

      Sets x and y from a tuple (r, phi) where r is the radial distance, and
      phi is the azimuthal angle.

      .. ## Vector2.from_polar ##

   .. method:: project

      | :sl:`projects a vector onto another.`
      | :sg:`project(Vector2, /) -> Vector2`

      Returns the projected vector. This is useful for collision detection in finding the components in a certain direction (e.g. in direction of the wall).
      For a more detailed explanation see `Wikipedia <https://en.wikipedia.org/wiki/Vector_projection>`_.

      .. versionaddedold:: 2.0.2

      .. ## Vector2.project ##


   .. method:: copy

      | :sl:`Returns a copy of itself.`
      | :sg:`copy() -> Vector2`

      Returns a new Vector2 having the same dimensions.

      .. versionaddedold:: 2.1.1

      .. ## Vector2.copy ##


   .. method:: clamp_magnitude

      | :sl:`Returns a copy of a vector with the magnitude clamped between max_length and min_length.`
      | :sg:`clamp_magnitude(max_length, /) -> Vector2`
      | :sg:`clamp_magnitude(min_length, max_length, /) -> Vector2`

      Returns a new copy of a vector with the magnitude clamped between
      ``max_length`` and ``min_length``. If only one argument is passed, it is
      taken to be the ``max_length``

      This function raises ``ValueError`` if ``min_length`` is greater than
      ``max_length``, or if either of these values are negative.

      .. versionadded:: 2.1.3

      .. versionchanged:: 2.4.0 It is now possible to use ``clamp_magnitude`` on a zero-vector as long as ``min_length``
         is unspecified or 0.

      .. note::
         Before pygame-ce 2.4.0, attempting to clamp a zero vector would always raise a ``ValueError``

      .. ## Vector2.clamp_magnitude ##


   .. method:: clamp_magnitude_ip

      | :sl:`Clamps the vector's magnitude between max_length and min_length`
      | :sg:`clamp_magnitude_ip(max_length, /) -> None`
      | :sg:`clamp_magnitude_ip(min_length, max_length, /) -> None`

      Clamps the vector's magnitude between ``max_length`` and ``min_length``.
      If only one argument is passed, it is taken to be the ``max_length``

      This function raises ``ValueError`` if ``min_length`` is greater than
      ``max_length``, or if either of these values are negative.

      .. versionadded:: 2.1.3

      .. versionchanged:: 2.4.0 It is now possible to use ``clamp_magnitude`` on a zero-vector as long as ``min_length``
         is unspecified or 0.

      .. note::
         Before pygame-ce 2.4.0, attempting to clamp a zero vector would always raise a ``ValueError``

      .. ## Vector2.clamp_magnitude_ip ##


   .. method:: update

      | :sl:`Sets the coordinates of the vector.`
      | :sg:`update() -> None`
      | :sg:`update(int) -> None`
      | :sg:`update(float) -> None`
      | :sg:`update(Vector2) -> None`
      | :sg:`update(x, y) -> None`
      | :sg:`update((x, y)) -> None`

      Sets coordinates x and y in place.

      .. versionaddedold:: 1.9.5

      .. ## Vector2.update ##


   .. attribute:: epsilon

      | :sl:`Determines the tolerance of vector calculations.`

      Both Vector classes have a value named ``epsilon`` that defaults to ``1e-6``.
      This value acts as a numerical margin in various methods to account for floating point
      arithmetic errors. Specifically, ``epsilon`` is used in the following places:

         * comparing Vectors (``==`` and ``!=``)
         * the ``is_normalized`` method (if the square of the length is within ``epsilon`` of 1, it's normalized)
         * slerping (a Vector with a length of ``<epsilon`` is considered a zero vector, and can't slerp with that)
         * reflection (can't reflect over the zero vector)
         * projection (can't project onto the zero vector)
         * rotation (only used when rotating by a multiple of 90 degrees)

      While it's possible to change ``epsilon`` for a specific instance of a Vector, all the other Vectors
      will retain the default value. Changing ``epsilon`` on a specific instance however could lead to some
      asymmetric behavior where symmetry would be expected, such as

      ::

         u = pygame.Vector2(0, 1)
         v = pygame.Vector2(0, 1.2)
         u.epsilon = 0.5 # don't set it nearly this large

         print(u == v) # >> True
         print(v == u) # >> False

      You'll probably never have to change ``epsilon`` from the default value, but in rare situations you might
      find that either the margin is too large or too small, in which case changing ``epsilon`` slightly
      might help you out.

   .. attribute:: angle

      | :sl:`Gives the angle of the vector in degrees, relative to the X-axis, normalized to the interval [-180, 180].`

      Read-only attribute representing the angle of the vector in degrees relative to the X-axis. This angle is normalized to
      the interval [-180, 180].

      Usage: Accessing `angle` provides the current angle of the vector in degrees within the predefined range of [-180, 180].

      .. versionadded:: 2.5.5

   .. attribute:: angle_rad

      | :sl:`Gives the angle of the vector in radians, relative to the X-axis, normalized to the interval [-π, π].`

      Read-only attribute representing the angle of the vector in radians relative to the X-axis. This value is equivalent
      to the `angle` attribute converted to radians and is normalized to the interval [-π, π].

      Usage: Accessing `angle_rad` provides the current angle of the vector in radians within the predefined range of [-π, π].

      .. versionadded:: 2.5.5

   .. ## pygame.math.Vector2 ##

.. class:: Vector3

   | :sl:`a 3-Dimensional Vector`
   | :sg:`Vector3() -> Vector3(0, 0, 0)`
   | :sg:`Vector3(int) -> Vector3`
   | :sg:`Vector3(float) -> Vector3`
   | :sg:`Vector3(Vector3) -> Vector3`
   | :sg:`Vector3(x, y, z) -> Vector3`
   | :sg:`Vector3((x, y, z)) -> Vector3`

   .. versionchanged:: 2.1.3
      Inherited methods of vector subclasses now correctly return an instance of the
      subclass instead of the superclass

   .. method:: dot

      | :sl:`calculates the dot- or scalar-product with the other vector`
      | :sg:`dot(Vector3, /) -> float`

      .. ## Vector3.dot ##

   .. method:: cross

      | :sl:`calculates the cross- or vector-product`
      | :sg:`cross(Vector3, /) -> Vector3`

      calculates the cross-product.

      .. ## Vector3.cross ##

   .. method:: magnitude

      | :sl:`returns the Euclidean magnitude of the vector.`
      | :sg:`magnitude() -> float`

      calculates the magnitude of the vector which follows from the
      theorem: ``vec.magnitude() == math.sqrt(vec.x**2 + vec.y**2 + vec.z**2)``

      .. ## Vector3.magnitude ##

   .. method:: magnitude_squared

      | :sl:`returns the squared Euclidean magnitude of the vector.`
      | :sg:`magnitude_squared() -> float`

      calculates the magnitude of the vector which follows from the
      theorem:
      ``vec.magnitude_squared() == vec.x**2 + vec.y**2 + vec.z**2``.
      This is faster than ``vec.magnitude()`` because it avoids the
      square root.

      .. ## Vector3.magnitude_squared ##

   .. method:: length

      | :sl:`returns the Euclidean length of the vector.`
      | :sg:`length() -> float`

      calculates the Euclidean length of the vector which follows from the
      Pythagorean theorem:
      ``vec.length() == math.sqrt(vec.x**2 + vec.y**2 + vec.z**2)``

      .. ## Vector3.length ##

   .. method:: length_squared

      | :sl:`returns the squared Euclidean length of the vector.`
      | :sg:`length_squared() -> float`

      calculates the Euclidean length of the vector which follows from the
      Pythagorean theorem:
      ``vec.length_squared() == vec.x**2 + vec.y**2 + vec.z**2``.
      This is faster than ``vec.length()`` because it avoids the square root.

      .. ## Vector3.length_squared ##

   .. method:: normalize

      | :sl:`returns a vector with the same direction but length 1.`
      | :sg:`normalize() -> Vector3`

      Returns a new vector that has ``length`` equal to ``1`` and the same
      direction as self. If the vector is the zero vector (i.e. has length
      ``0`` thus no direction) a ``ValueError`` is raised.

      .. ## Vector3.normalize ##

   .. method:: normalize_ip

      | :sl:`normalizes the vector in place so that its length is 1.`
      | :sg:`normalize_ip() -> None`

      Normalizes the vector so that it has ``length`` equal to ``1``. The
      direction of the vector is not changed. If the vector is the zero vector
      (i.e. has length ``0`` thus no direction) a ``ValueError`` is raised.

      .. ## Vector3.normalize_ip ##

   .. method:: is_normalized

      | :sl:`tests if the vector is normalized i.e. has length == 1.`
      | :sg:`is_normalized() -> bool`

      Returns True if the vector has ``length`` equal to ``1``. Otherwise it
      returns ``False``.

      .. ## Vector3.is_normalized ##

   .. method:: scale_to_length

      | :sl:`scales the vector to a given length.`
      | :sg:`scale_to_length(float, /) -> None`

      Scales the vector so that it has the given length. The direction of the
      vector is not changed. You can also scale to length ``0``. If the vector
      is the zero vector (i.e. has length ``0`` thus no direction) a
      ``ValueError`` is raised.

      .. ## Vector3.scale_to_length ##

   .. method:: reflect

      | :sl:`returns a vector reflected of a given normal.`
      | :sg:`reflect(Vector3, /) -> Vector3`

      Returns a new vector that points in the direction as if self would bounce
      of a surface characterized by the given surface normal. The length of the
      new vector is the same as self's.

      .. ## Vector3.reflect ##

   .. method:: reflect_ip

      | :sl:`reflect the vector of a given normal in place.`
      | :sg:`reflect_ip(Vector3, /) -> None`

      Changes the direction of self as if it would have been reflected of a
      surface with the given surface normal.

      .. ## Vector3.reflect_ip ##

   .. method:: distance_to

      | :sl:`calculates the Euclidean distance to a given vector.`
      | :sg:`distance_to(Vector3, /) -> float`

      .. ## Vector3.distance_to ##

   .. method:: distance_squared_to

      | :sl:`calculates the squared Euclidean distance to a given vector.`
      | :sg:`distance_squared_to(Vector3, /) -> float`

      .. ## Vector3.distance_squared_to ##

   .. method:: move_towards

      | :sl:`returns a vector moved toward the target by a given distance.`
      | :sg:`move_towards(Vector3, float, /) -> Vector3`

      Returns a Vector which is moved towards the given Vector by a given
      distance and does not overshoot past its target Vector.
      The first parameter determines the target Vector, while the second
      parameter determines the delta distance. If the distance is in the
      negatives, then it will move away from the target Vector.

      .. versionadded:: 2.1.3

      .. ## Vector3.move_towards ##

   .. method:: move_towards_ip

      | :sl:`moves the vector toward its target at a given distance.`
      | :sg:`move_towards_ip(Vector3, float, /) -> None`

      Moves itself toward the given Vector at a given distance and does not
      overshoot past its target Vector.
      The first parameter determines the target Vector, while the second
      parameter determines the delta distance. If the distance is in the
      negatives, then it will move away from the target Vector.

      .. versionadded:: 2.1.3

      .. ## Vector3.move_towards_ip ##

   .. method:: lerp

      | :sl:`returns a linear interpolation to the given vector.`
      | :sg:`lerp(Vector3, float, /) -> Vector3`

      Returns a Vector which is a linear interpolation between self and the
      given Vector. The second parameter determines how far between self and
      other the result is going to be. It must be a value between ``0`` and
      ``1``, where ``0`` means self and ``1`` means other will be returned.

      .. ## Vector3.lerp ##

   .. method:: slerp

      | :sl:`returns a spherical interpolation to the given vector.`
      | :sg:`slerp(Vector3, float, /) -> Vector3`

      Calculates the spherical interpolation from self to the given Vector. The
      second argument - often called t - must be in the range ``[-1, 1]``. It
      parametrizes where - in between the two vectors - the result should be.
      If a negative value is given the interpolation will not take the
      complement of the shortest path.

      .. ## Vector3.slerp ##

   .. method:: smoothstep

      | :sl:`returns a smooth interpolation to the given vector.`
      | :sg:`smoothstep(Vector3, value, /) -> Vector3`

      Returns a Vector which is a smooth interpolation between self and the
      given Vector. This means that the interpolation follows an s-shaped curve, with
      change happening more slowly near the limits (0.0 and 1.0) and faster in the middle.
      The second parameter determines how far between the two vectors the result is going to be.
      Value is clamped to ``[0, 1]``.

      The formula is:

      ``a * (1 - interp) + b * interp``

      where:

      ``interp = value * value * (3 - 2 * value)``

      .. versionadded:: 2.4.0

      .. ## Vector3.smoothstep ##

   .. method:: elementwise

      | :sl:`The next operation will be performed elementwise.`
      | :sg:`elementwise() -> VectorElementwiseProxy`

      Applies the following operation to each element of the vector.

      .. ## Vector3.elementwise ##

   .. method:: rotate

      | :sl:`rotates a vector by a given angle in degrees.`
      | :sg:`rotate(angle, Vector3, /) -> Vector3`

      Returns a vector which has the same length as self but is rotated
      counterclockwise by the given angle in degrees around the given axis.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector3.rotate ##

   .. method:: rotate_rad

      | :sl:`rotates a vector by a given angle in radians.`
      | :sg:`rotate_rad(angle, Vector3, /) -> Vector3`

      Returns a vector which has the same length as self but is rotated
      counterclockwise by the given angle in radians around the given axis.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.0.0

      .. ## Vector3.rotate_rad ##

   .. method:: rotate_ip

      | :sl:`rotates the vector by a given angle in degrees in place.`
      | :sg:`rotate_ip(angle, Vector3, /) -> None`

      Rotates the vector counterclockwise around the given axis by the given
      angle in degrees. The length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector3.rotate_ip ##

   .. method:: rotate_ip_rad

      | :sl:`rotates the vector by a given angle in radians in place.`
      | :sg:`rotate_ip_rad(angle, Vector3, /) -> None`

      DEPRECATED: Use rotate_rad_ip() instead.

      .. versionaddedold:: 2.0.0
      .. deprecatedold:: 2.1.1

      .. ## Vector3.rotate_ip_rad ##

   .. method:: rotate_rad_ip

      | :sl:`rotates the vector by a given angle in radians in place.`
      | :sg:`rotate_rad_ip(angle, Vector3, /) -> None`

      Rotates the vector counterclockwise around the given axis by the given
      angle in radians. The length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.1.1

      .. ## Vector3.rotate_rad_ip ##

   .. method:: rotate_x

      | :sl:`rotates a vector around the x-axis by the angle in degrees.`
      | :sg:`rotate_x(angle, /) -> Vector3`

      Returns a vector which has the same length as self but is rotated
      counterclockwise around the x-axis by the given angle in degrees.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector3.rotate_x ##

   .. method:: rotate_x_rad

      | :sl:`rotates a vector around the x-axis by the angle in radians.`
      | :sg:`rotate_x_rad(angle, /) -> Vector3`

      Returns a vector which has the same length as self but is rotated
      counterclockwise around the x-axis by the given angle in radians.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.0.0

      .. ## Vector3.rotate_x_rad ##

   .. method:: rotate_x_ip

      | :sl:`rotates the vector around the x-axis by the angle in degrees in place.`
      | :sg:`rotate_x_ip(angle, /) -> None`

      Rotates the vector counterclockwise around the x-axis by the given angle
      in degrees. The length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector3.rotate_x_ip ##

   .. method:: rotate_x_ip_rad

      | :sl:`rotates the vector around the x-axis by the angle in radians in place.`
      | :sg:`rotate_x_ip_rad(angle, /) -> None`

      DEPRECATED: Use rotate_x_rad_ip() instead.

      .. versionaddedold:: 2.0.0
      .. deprecatedold:: 2.1.1

      .. ## Vector3.rotate_x_ip_rad ##

   .. method:: rotate_x_rad_ip

      | :sl:`rotates the vector around the x-axis by the angle in radians in place.`
      | :sg:`rotate_x_rad_ip(angle, /) -> None`

      Rotates the vector counterclockwise around the x-axis by the given angle
      in radians. The length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.1.1

      .. ## Vector3.rotate_x_rad_ip ##

   .. method:: rotate_y

      | :sl:`rotates a vector around the y-axis by the angle in degrees.`
      | :sg:`rotate_y(angle, /) -> Vector3`

      Returns a vector which has the same length as self but is rotated
      counterclockwise around the y-axis by the given angle in degrees.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector3.rotate_y ##

   .. method:: rotate_y_rad

      | :sl:`rotates a vector around the y-axis by the angle in radians.`
      | :sg:`rotate_y_rad(angle, /) -> Vector3`

      Returns a vector which has the same length as self but is rotated
      counterclockwise around the y-axis by the given angle in radians.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.0.0

      .. ## Vector3.rotate_y_rad ##

   .. method:: rotate_y_ip

      | :sl:`rotates the vector around the y-axis by the angle in degrees in place.`
      | :sg:`rotate_y_ip(angle, /) -> None`

      Rotates the vector counterclockwise around the y-axis by the given angle
      in degrees. The length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector3.rotate_y_ip ##

   .. method:: rotate_y_ip_rad

      | :sl:`rotates the vector around the y-axis by the angle in radians in place.`
      | :sg:`rotate_y_ip_rad(angle, /) -> None`

      DEPRECATED: Use rotate_y_rad_ip() instead.

      .. versionaddedold:: 2.0.0
      .. deprecatedold:: 2.1.1
      .. versionchanged:: 2.5.8
         Fixed a regression introduced in 2.1.1 where this method incorrectly rotated
         around the X-axis instead of the Y-axis. In versions prior to 2.1.1, rotation
         around the Y-axis worked correctly.

      .. ## Vector3.rotate_y_ip_rad ##

   .. method:: rotate_y_rad_ip

      | :sl:`rotates the vector around the y-axis by the angle in radians in place.`
      | :sg:`rotate_y_rad_ip(angle, /) -> None`

      Rotates the vector counterclockwise around the y-axis by the given angle
      in radians. The length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.1.1

      .. ## Vector3.rotate_y_rad_ip ##

   .. method:: rotate_z

      | :sl:`rotates a vector around the z-axis by the angle in degrees.`
      | :sg:`rotate_z(angle, /) -> Vector3`

      Returns a vector which has the same length as self but is rotated
      counterclockwise around the z-axis by the given angle in degrees.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector3.rotate_z ##

   .. method:: rotate_z_rad

      | :sl:`rotates a vector around the z-axis by the angle in radians.`
      | :sg:`rotate_z_rad(angle, /) -> Vector3`

      Returns a vector which has the same length as self but is rotated
      counterclockwise around the z-axis by the given angle in radians.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.0.0

      .. ## Vector3.rotate_z_rad ##

   .. method:: rotate_z_ip

      | :sl:`rotates the vector around the z-axis by the angle in degrees in place.`
      | :sg:`rotate_z_ip(angle, /) -> None`

      Rotates the vector counterclockwise around the z-axis by the given angle
      in degrees. The length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. ## Vector3.rotate_z_ip ##

   .. method:: rotate_z_ip_rad

      | :sl:`rotates the vector around the z-axis by the angle in radians in place.`
      | :sg:`rotate_z_ip_rad(angle, /) -> None`

      DEPRECATED: Use rotate_z_rad_ip() instead.

      .. versionaddedold:: 2.0.0
      .. deprecatedold:: 2.1.1
      .. versionchanged:: 2.5.8
         Fixed a regression introduced in 2.1.1 where this method incorrectly rotated
         around the X-axis instead of the Z-axis. In versions prior to 2.1.1, rotation
         around the Z-axis worked correctly.

      .. ## Vector3.rotate_z_ip_rad ##

   .. method:: rotate_z_rad_ip

      | :sl:`rotates the vector around the z-axis by the angle in radians in place.`
      | :sg:`rotate_z_rad_ip(angle, /) -> None`

      Rotates the vector counterclockwise around the z-axis by the given angle
      in radians. The length of the vector is not changed.
      (Note that due to pygame's inverted y coordinate system, the rotation
      will look clockwise if displayed).

      .. versionaddedold:: 2.1.1

      .. ## Vector3.rotate_z_rad_ip ##

   .. method:: angle_to

      | :sl:`calculates the angle to a given vector in degrees.`
      | :sg:`angle_to(Vector3, /) -> float`

      Returns the angle between self and the given vector.

      .. ## Vector3.angle_to ##

   .. method:: as_spherical

      | :sl:`returns a tuple with radial distance, inclination and azimuthal angle.`
      | :sg:`as_spherical() -> (r, theta, phi)`

      Returns a tuple ``(r, theta, phi)`` where r is the radial distance, theta is
      the inclination angle and phi is the azimuthal angle.

      .. ## Vector3.as_spherical ##

   .. method:: from_spherical

      | :sl:`Sets x, y and z from a spherical coordinates 3-tuple.`
      | :sg:`from_spherical((r, theta, phi), /) -> None`

      Sets x, y and z from a tuple ``(r, theta, phi)`` where r is the radial
      distance, theta is the inclination angle and phi is the azimuthal angle.

      .. ## Vector3.from_spherical ##

   .. method:: project

      | :sl:`projects a vector onto another.`
      | :sg:`project(Vector3, /) -> Vector3`

      Returns the projected vector. This is useful for collision detection in finding the components in a certain direction (e.g. in direction of the wall).
      For a more detailed explanation see `Wikipedia <https://en.wikipedia.org/wiki/Vector_projection>`_.

      .. versionaddedold:: 2.0.2

      .. ## Vector3.project ##

   .. method:: copy

      | :sl:`Returns a copy of itself.`
      | :sg:`copy() -> Vector3`

      Returns a new Vector3 having the same dimensions.

      .. versionaddedold:: 2.1.1

      .. ## Vector3.copy ##


   .. method:: clamp_magnitude

      | :sl:`Returns a copy of a vector with the magnitude clamped between max_length and min_length.`
      | :sg:`clamp_magnitude(max_length, /) -> Vector3`
      | :sg:`clamp_magnitude(min_length, max_length, /) -> Vector3`

      Returns a new copy of a vector with the magnitude clamped between
      ``max_length`` and ``min_length``. If only one argument is passed, it is
      taken to be the ``max_length``

      This function raises ``ValueError`` if ``min_length`` is greater than
      ``max_length``, or if either of these values are negative.

      .. versionadded:: 2.1.3

      .. versionchanged:: 2.4.0 It is now possible to use ``clamp_magnitude`` on a zero-vector as long as ``min_length``
         is unspecified or 0.

      .. note::
         Before pygame-ce 2.4.0, attempting to clamp a zero vector would always raise a ``ValueError``

      .. ## Vector3.clamp_magnitude ##


   .. method:: clamp_magnitude_ip

      | :sl:`Clamps the vector's magnitude between max_length and min_length`
      | :sg:`clamp_magnitude_ip(max_length, /) -> None`
      | :sg:`clamp_magnitude_ip(min_length, max_length, /) -> None`

      Clamps the vector's magnitude between ``max_length`` and ``min_length``.
      If only one argument is passed, it is taken to be the ``max_length``

      This function raises ``ValueError`` if ``min_length`` is greater than
      ``max_length``, or if either of these values are negative.

      .. versionadded:: 2.1.3

      .. versionchanged:: 2.4.0 It is now possible to use ``clamp_magnitude`` on a zero-vector as long as ``min_length``
         is unspecified or 0.

      .. note::
         Before pygame-ce 2.4.0, attempting to clamp a zero vector would always raise a ``ValueError``

      .. ## Vector3.clamp_magnitude_ip ##

   .. method:: update

      | :sl:`Sets the coordinates of the vector.`
      | :sg:`update() -> None`
      | :sg:`update(int) -> None`
      | :sg:`update(float) -> None`
      | :sg:`update(Vector3) -> None`
      | :sg:`update(x, y, z) -> None`
      | :sg:`update((x, y, z)) -> None`

      Sets coordinates x, y, and z in place.

      .. versionaddedold:: 1.9.5

      .. ## Vector3.update ##

   .. attribute:: epsilon

      | :sl:`Determines the tolerance of vector calculations.`

      With lengths within this number, vectors are considered equal. For more information see :attr:`pygame.math.Vector2.epsilon`

   .. ##  ##

   .. ## pygame.math.Vector3 ##

.. class:: Vector4

   | :sl:`a 4-Dimensional Vector`
   | :sg:`Vector4() -> Vector4(0, 0, 0, 0)`
   | :sg:`Vector4(int) -> Vector4`
   | :sg:`Vector4(float) -> Vector4`
   | :sg:`Vector4(Vector4) -> Vector4`
   | :sg:`Vector4(x, y, z, w) -> Vector4`
   | :sg:`Vector4((x, y, z, w)) -> Vector4`

   Vector4 is the four-dimensional counterpart to :class:`Vector2` and
   :class:`Vector3`. It stores four numeric components named ``x``, ``y``,
   ``z`` and ``w`` and provides the dimension-independent subset of the pygame
   vector API. It is a natural Python representation of a shader ``float4``
   value and of homogeneous coordinates for ``Matrix4x4`` multiplication.

   Unlike :class:`Vector2` and :class:`Vector3`, ``Vector4`` does not provide
   ``cross``, ``rotate`` or coordinate-system conversions such as
   ``as_spherical``/``as_polar``, because those operations are specific to two
   and three dimensions.

   Construction requires either zero arguments (the zero vector), a single
   scalar (broadcast to all four components), a single sequence or ``Vector4``
   of exactly four components, a string matching its representation, or four
   explicit scalar components. A :class:`Vector3` is **not** implicitly
   promoted to a ``Vector4``; supply the fourth component explicitly.

   .. versionadded:: 3.0.0

   .. method:: dot

      | :sl:`calculates the dot- or scalar-product with the other vector`
      | :sg:`dot(Vector4, /) -> float`

      .. ## Vector4.dot ##

   .. method:: magnitude

      | :sl:`returns the Euclidean magnitude of the vector.`
      | :sg:`magnitude() -> float`

      calculates the magnitude of the vector which follows from the
      theorem:
      ``vec.magnitude() == math.sqrt(vec.x**2 + vec.y**2 + vec.z**2 + vec.w**2)``

      .. ## Vector4.magnitude ##

   .. method:: magnitude_squared

      | :sl:`returns the squared Euclidean magnitude of the vector.`
      | :sg:`magnitude_squared() -> float`

      calculates the magnitude of the vector which follows from the
      theorem:
      ``vec.magnitude_squared() == vec.x**2 + vec.y**2 + vec.z**2 + vec.w**2``.
      This is faster than ``vec.magnitude()`` because it avoids the
      square root.

      .. ## Vector4.magnitude_squared ##

   .. method:: length

      | :sl:`returns the Euclidean length of the vector.`
      | :sg:`length() -> float`

      calculates the Euclidean length of the vector which follows from the
      theorem:
      ``vec.length() == math.sqrt(vec.x**2 + vec.y**2 + vec.z**2 + vec.w**2)``

      .. ## Vector4.length ##

   .. method:: length_squared

      | :sl:`returns the squared Euclidean length of the vector.`
      | :sg:`length_squared() -> float`

      calculates the Euclidean length of the vector which follows from the
      theorem:
      ``vec.length_squared() == vec.x**2 + vec.y**2 + vec.z**2 + vec.w**2``.
      This is faster than ``vec.length()`` because it avoids the square root.

      .. ## Vector4.length_squared ##

   .. method:: normalize

      | :sl:`returns a vector with the same direction but length 1.`
      | :sg:`normalize() -> Vector4`

      Returns a new vector that has ``length == 1`` and the same direction as
      self. If the vector is the zero vector (i.e. has length ``0`` thus no
      direction) a ``ValueError`` is raised.

      .. ## Vector4.normalize ##

   .. method:: normalize_ip

      | :sl:`normalizes the vector in place so that its length is 1.`
      | :sg:`normalize_ip() -> None`

      Normalizes the vector so that it has ``length == 1``. The direction of
      the vector is not changed. If the vector is the zero vector (i.e. has
      length ``0`` thus no direction) a ``ValueError`` is raised.

      .. ## Vector4.normalize_ip ##

   .. method:: is_normalized

      | :sl:`tests if the vector is normalized i.e. has length == 1.`
      | :sg:`is_normalized() -> bool`

      Returns True if the vector has ``length == 1``. Otherwise it returns
      False.

      .. ## Vector4.is_normalized ##

   .. method:: scale_to_length

      | :sl:`scales the vector to a given length.`
      | :sg:`scale_to_length(float, /) -> None`

      Scales the vector so that it has the given length. The direction of the
      vector is not changed. You can also scale to length 0. If the vector is
      the zero vector (i.e. has length 0 thus no direction) a ValueError is
      raised.

      .. ## Vector4.scale_to_length ##

   .. method:: reflect

      | :sl:`returns a vector reflected of a given normal.`
      | :sg:`reflect(Vector4, /) -> Vector4`

      Returns a new vector that points in the direction as if self would bounce
      of a surface characterized by the given surface normal. The length of the
      new vector is the same as self's.

      .. ## Vector4.reflect ##

   .. method:: reflect_ip

      | :sl:`reflect the vector of a given normal in place.`
      | :sg:`reflect_ip(Vector4, /) -> None`

      Changes the direction of self as if it would have been reflected of a
      surface with the given surface normal.

      .. ## Vector4.reflect_ip ##

   .. method:: distance_to

      | :sl:`calculates the Euclidean distance to a given vector.`
      | :sg:`distance_to(Vector4, /) -> float`

      .. ## Vector4.distance_to ##

   .. method:: distance_squared_to

      | :sl:`calculates the squared Euclidean distance to a given vector.`
      | :sg:`distance_squared_to(Vector4, /) -> float`

      .. ## Vector4.distance_squared_to ##

   .. method:: move_towards

      | :sl:`returns a vector moved toward the target by a given distance.`
      | :sg:`move_towards(Vector4, float, /) -> Vector4`

      Returns a Vector which is moved towards the given Vector by a given
      distance and does not overshoot past its target Vector. The first
      parameter determines the target Vector, while the second parameter
      determines the delta distance. If the distance is in the negatives, then
      it will move away from the target Vector.

      .. ## Vector4.move_towards ##

   .. method:: move_towards_ip

      | :sl:`moves the vector toward its target at a given distance.`
      | :sg:`move_towards_ip(Vector4, float, /) -> None`

      Moves itself toward the given Vector at a given distance and does not
      overshoot past its target Vector. The first parameter determines the
      target Vector, while the second parameter determines the delta distance.
      If the distance is in the negatives, then it will move away from the
      target Vector.

      .. ## Vector4.move_towards_ip ##

   .. method:: lerp

      | :sl:`returns a linear interpolation to the given vector.`
      | :sg:`lerp(Vector4, float, /) -> Vector4`

      Returns a Vector which is a linear interpolation between self and the
      given Vector. The second parameter determines how far between self and
      other the result is going to be. It must be a value between 0 and 1,
      where 0 means self and 1 means other will be returned.

      .. ## Vector4.lerp ##

   .. method:: slerp

      | :sl:`returns a spherical interpolation to the given vector.`
      | :sg:`slerp(Vector4, float, /) -> Vector4`

      Calculates the spherical interpolation from self to the given Vector.
      The second argument - often called t - must be in the range ``[-1, 1]``.
      It parametrizes where to interpolate between the two vectors. If ``t`` is
      negative the interpolation follows the long arc of the great circle
      spanned by the two vectors. The magnitude is linearly interpolated
      between the two input magnitudes. Interpolating between two exactly
      opposite (antiparallel) vectors is undefined and raises a ``ValueError``.

      .. ## Vector4.slerp ##

   .. method:: smoothstep

      | :sl:`returns a smooth interpolation to the given vector.`
      | :sg:`smoothstep(Vector4, value, /) -> Vector4`

      Returns a Vector which is a smooth interpolation between self and the
      given Vector. This works like :meth:`lerp` but the interpolation is
      smoother near the endpoints. The second parameter determines how far
      between self and other the result is going to be. It must be a value
      between 0 and 1.

      .. ## Vector4.smoothstep ##

   .. method:: elementwise

      | :sl:`The next operation will be performed elementwise.`
      | :sg:`elementwise() -> VectorElementwiseProxy`

      Applies the following operation to each element of the vector.

      .. ## Vector4.elementwise ##

   .. method:: angle_to

      | :sl:`calculates the angle to a given vector in degrees.`
      | :sg:`angle_to(Vector4, /) -> float`

      Returns the angle between self and the given vector in degrees.

      .. ## Vector4.angle_to ##

   .. method:: project

      | :sl:`projects a vector onto another.`
      | :sg:`project(Vector4, /) -> Vector4`

      Returns the projection of self onto the passed vector ``v``, computed as
      ``self.dot(v) / v.dot(v) * v``. Projecting onto a zero-length vector
      raises a ValueError.

      .. ## Vector4.project ##

   .. method:: copy

      | :sl:`Returns a copy of itself.`
      | :sg:`copy() -> Vector4`

      Returns a new Vector4 having the same dimensions and values as self.

      .. ## Vector4.copy ##

   .. method:: clamp_magnitude

      | :sl:`Returns a copy of a vector with the magnitude clamped between max_length and min_length.`
      | :sg:`clamp_magnitude(max_length, /) -> Vector4`
      | :sg:`clamp_magnitude(min_length, max_length, /) -> Vector4`

      Returns a new copy of a vector with the magnitude clamped between
      ``max_length`` and ``min_length``. If only one argument is passed it is
      taken to be the ``max_length``.

      This function raises ``ValueError`` if ``min_length`` is greater than
      ``max_length``, or if either of these values are negative.

      .. ## Vector4.clamp_magnitude ##

   .. method:: clamp_magnitude_ip

      | :sl:`Clamps the vector's magnitude between max_length and min_length`
      | :sg:`clamp_magnitude_ip(max_length, /) -> None`
      | :sg:`clamp_magnitude_ip(min_length, max_length, /) -> None`

      Clamps the vector's magnitude between ``max_length`` and ``min_length``
      in place. If only one argument is passed it is taken to be the
      ``max_length``.

      This function raises ``ValueError`` if ``min_length`` is greater than
      ``max_length``, or if either of these values are negative.

      .. ## Vector4.clamp_magnitude_ip ##

   .. method:: update

      | :sl:`Sets the coordinates of the vector.`
      | :sg:`update() -> None`
      | :sg:`update(int) -> None`
      | :sg:`update(float) -> None`
      | :sg:`update(Vector4) -> None`
      | :sg:`update(x, y, z, w) -> None`
      | :sg:`update((x, y, z, w)) -> None`

      Sets coordinates x, y, z, and w in place.

      .. ## Vector4.update ##

   .. attribute:: epsilon

      | :sl:`Determines the tolerance of vector calculations.`

      With lengths within this number, vectors are considered equal. For more information see :attr:`pygame.math.Vector2.epsilon`

   .. ##  ##

   .. ## pygame.math.Vector4 ##

.. class:: Matrix4x4

   | :sl:`a 4x4 matrix for 3D transformations and general linear algebra`
   | :sg:`Matrix4x4() -> Matrix4x4`
   | :sg:`Matrix4x4(Matrix4x4) -> Matrix4x4`
   | :sg:`Matrix4x4(sequence_of_16_numbers) -> Matrix4x4`
   | :sg:`Matrix4x4(m00, m01, ..., m33) -> Matrix4x4`

   ``Matrix4x4`` is a general-purpose 4x4 matrix of double-precision floats. It
   is useful for homogeneous-coordinate mathematics, 3D model/view/projection
   transformations and camera mathematics, and it is a natural Python
   representation of a shader ``float4x4`` value. It is a pure mathematical
   type and has no dependency on the GPU subsystem.

   **Conventions.** The class uses a single, explicit convention throughout:

   * **Column vectors:** a vector is transformed with ``result = matrix @ vector``,
     and matrices compose as ``mvp = projection @ view @ model``.
   * **Indexing:** ``m[row, column]`` with both indices in ``0..3``. An affine
     transform stores its translation in ``m[0, 3]``, ``m[1, 3]``, ``m[2, 3]``
     and has ``0, 0, 0, 1`` as its final row.
   * **Storage order:** logically row-major. :meth:`to_tuple`, :meth:`to_list`
     and the sequence constructor use the flat order
     ``m00, m01, m02, m03, m10, ... m33``. Iteration, however, is row-oriented
     (see **Sequence semantics** below).
   * **Angles** are given in **degrees**.
   * **Coordinate system:** left-handed, with camera forward ``+Z`` and a
     clip-space depth range of ``z`` in ``[0, 1]`` (``near`` maps to ``0``,
     ``far`` maps to ``1``). This matches the SDL_GPU convention rather than
     OpenGL's ``[-1, 1]`` depth.

   Constructing with no arguments returns the identity matrix. A single
   argument may be another ``Matrix4x4`` (copied) or an iterable of exactly 16
   numbers; alternatively 16 numeric arguments may be given directly.

   Equality is fuzzy: two matrices compare equal when every element differs by
   no more than :attr:`epsilon`. The ``equals`` method uses the same inclusive
   ``<=`` rule. ``Matrix4x4`` is unhashable and mutable.

   **Sequence semantics.** ``Matrix4x4`` follows a row-oriented model similar to
   a 2-D NumPy array of shape ``(4, 4)``. ``len(m)`` is ``4`` (the number of
   rows), and iterating a matrix yields its 4 rows as independent
   :class:`Vector4` instances (so ``list(m)`` gives four ``Vector4`` rows).
   Integer indexing is likewise a row accessor: ``m[i]`` returns row ``i`` as a
   :class:`Vector4` for ``i`` in ``0..3`` (negative indices count from the
   end), while element access uses the two-index form ``m[row, column]``. For
   the flat 16-element view (row-major), use :meth:`to_tuple` or
   :meth:`to_list`.

   .. note::
      ``numpy.array(matrix)`` does **not** automatically produce a ``(4, 4)``
      array, because ``Matrix4x4`` uses mapping-style ``m[row, column]``
      indexing rather than the flat sequence protocol. To get a NumPy array,
      convert explicitly, e.g. ``numpy.array(m.to_list()).reshape(4, 4)`` or
      ``numpy.array([list(row) for row in m])``.

   This type requires :class:`Vector3` and :class:`Vector4`.

   .. versionadded:: 3.0.0

   .. classmethod:: identity

      | :sl:`returns a new identity matrix.`
      | :sg:`identity() -> Matrix4x4`

      Returns a new, independent identity matrix.

      .. ## Matrix4x4.identity ##

   .. classmethod:: zero

      | :sl:`returns a new matrix filled with zeros.`
      | :sg:`zero() -> Matrix4x4`

      .. ## Matrix4x4.zero ##

   .. classmethod:: diagonal

      | :sl:`returns a new diagonal matrix.`
      | :sg:`diagonal(x, y, z, w) -> Matrix4x4`

      Returns a matrix whose main diagonal is ``(x, y, z, w)`` and whose other
      elements are zero.

      .. ## Matrix4x4.diagonal ##

   .. classmethod:: from_rows

      | :sl:`builds a matrix from its four rows.`
      | :sg:`from_rows(row0, row1, row2, row3) -> Matrix4x4`

      Each argument must be a :class:`Vector4` or a sequence of four numbers,
      supplying one row of the matrix.

      .. ## Matrix4x4.from_rows ##

   .. classmethod:: from_columns

      | :sl:`builds a matrix from its four columns.`
      | :sg:`from_columns(column0, column1, column2, column3) -> Matrix4x4`

      Each argument must be a :class:`Vector4` or a sequence of four numbers,
      supplying one column of the matrix.

      .. ## Matrix4x4.from_columns ##

   .. classmethod:: translation

      | :sl:`builds a translation matrix.`
      | :sg:`translation(offset) -> Matrix4x4`

      Returns an affine translation matrix for the given ``offset``
      (a :class:`Vector3` or a sequence of three numbers), with the offset in
      ``m[0, 3]``, ``m[1, 3]``, ``m[2, 3]``. This is the noun counterpart of the
      :meth:`translate` operation; ``Matrix4x4.translation(v)`` equals
      ``Matrix4x4().translate(v)`` but avoids building an intermediate identity.

      .. ## Matrix4x4.translation ##

   .. classmethod:: rotation_axis_angle

      | :sl:`builds a rotation matrix about an arbitrary axis.`
      | :sg:`rotation_axis_angle(axis, angle) -> Matrix4x4`

      Returns a rotation matrix about ``axis`` (a :class:`Vector3` or a
      sequence of three numbers, normalized internally) by ``angle`` degrees.
      In this left-handed convention a positive angle is a **clockwise**
      rotation when viewed from the ``+axis`` looking toward the origin. A
      zero-length axis raises a ``ValueError``. Noun counterpart of
      :meth:`rotate`.

      .. ## Matrix4x4.rotation_axis_angle ##

   .. classmethod:: scaling

      | :sl:`builds a scale matrix.`
      | :sg:`scaling(scalar) -> Matrix4x4`
      | :sg:`scaling(vector3) -> Matrix4x4`

      Returns a scale matrix. A single number produces a uniform scale; a
      :class:`Vector3` or a sequence of three numbers produces a non-uniform
      scale along the ``x``, ``y`` and ``z`` axes. Named ``scaling`` (not
      ``scale``) so it doesn't collide with the :meth:`scale` operation. Noun
      counterpart of :meth:`scale`.

      .. ## Matrix4x4.scaling ##

   .. classmethod:: look_at

      | :sl:`builds a world-to-view (camera) matrix.`
      | :sg:`look_at(eye, target, up) -> Matrix4x4`

      Returns a left-handed world-to-view matrix that places a camera at
      ``eye`` looking toward ``target`` with the given ``up`` direction. The
      basis is ``forward = normalize(target - eye)``,
      ``right = normalize(cross(up, forward))`` and
      ``up' = cross(forward, right)``. A ``ValueError`` is raised if ``eye``
      equals ``target`` or if ``up`` is parallel to the view direction. All
      arguments are :class:`Vector3` or sequences of three numbers.

      .. ## Matrix4x4.look_at ##

   .. classmethod:: perspective

      | :sl:`builds a perspective projection matrix.`
      | :sg:`perspective(fov, aspect, near, far) -> Matrix4x4`

      Returns a left-handed perspective projection matrix. ``fov`` is the
      **vertical** field of view in **degrees**, ``aspect`` is the width divided
      by the height of the viewport, and ``near``/``far`` are the clip-plane
      distances. The resulting clip-space depth range is ``z`` in ``[0, 1]``
      (``near`` maps to ``0``, ``far`` maps to ``1``).

      Domain constraints (each raises ``ValueError`` if violated): ``fov`` must
      be in the open interval ``(0, 180)`` degrees, ``aspect`` must be
      positive, ``near`` must be positive, and ``far`` must be greater than
      ``near``.

      .. ## Matrix4x4.perspective ##

   .. classmethod:: orthographic

      | :sl:`builds an orthographic projection matrix.`
      | :sg:`orthographic(left, right, bottom, top, near, far) -> Matrix4x4`

      Returns a left-handed orthographic projection matrix. The depth
      convention matches :meth:`perspective`: ``z`` in ``[0, 1]`` with ``near``
      mapping to ``0`` and ``far`` mapping to ``1``.

      Domain constraints (each raises ``ValueError`` if violated): ``left`` and
      ``right`` must differ, ``bottom`` and ``top`` must differ, and ``far``
      must be greater than ``near``.

      .. ## Matrix4x4.orthographic ##

   .. attribute:: rows

      | :sl:`the number of rows (always 4).`

      .. ## Matrix4x4.rows ##

   .. attribute:: columns

      | :sl:`the number of columns (always 4).`

      .. ## Matrix4x4.columns ##

   .. attribute:: trace

      | :sl:`the sum of the diagonal elements.`

      Equal to ``m[0,0] + m[1,1] + m[2,2] + m[3,3]``.

      .. ## Matrix4x4.trace ##

   .. attribute:: determinant

      | :sl:`the determinant of the matrix.`

      .. ## Matrix4x4.determinant ##

   .. attribute:: epsilon

      | :sl:`the tolerance used for equality comparisons.`

      Two matrices are considered equal when every corresponding element
      differs by no more than this value (inclusive). Defaults to ``1e-6``.

      .. ## Matrix4x4.epsilon ##

   .. method:: is_identity

      | :sl:`tests whether the matrix is approximately the identity.`
      | :sg:`is_identity(tolerance=...) -> bool`

      .. ## Matrix4x4.is_identity ##

   .. method:: is_affine

      | :sl:`tests whether the matrix is an affine transform.`
      | :sg:`is_affine(tolerance=...) -> bool`

      Returns ``True`` when the final row is approximately ``0, 0, 0, 1``.

      .. ## Matrix4x4.is_affine ##

   .. method:: is_orthogonal

      | :sl:`tests whether the matrix is orthogonal.`
      | :sg:`is_orthogonal(tolerance=...) -> bool`

      Returns ``True`` when ``Mᵀ M`` is approximately the identity, i.e. the
      columns are orthonormal. Note that an orthogonal matrix may represent a
      rotation *or* a reflection (or a combination), so this is not by itself a
      test for a pure rotation. Matrices containing ``NaN`` are never
      orthogonal.

      .. ## Matrix4x4.is_orthogonal ##

   .. method:: transform_point

      | :sl:`transforms a point, applying translation and perspective divide.`
      | :sg:`transform_point(point) -> Vector3`

      Treats ``point`` as the homogeneous point ``(x, y, z, 1)``, multiplies it
      by the matrix and divides the result by the resulting ``w`` component,
      returning a :class:`Vector3`. This is identical to ``matrix @ vector3``.
      A resulting ``w`` of zero raises a ``ValueError``.

      .. ## Matrix4x4.transform_point ##

   .. method:: transform_direction

      | :sl:`transforms a direction, ignoring translation.`
      | :sg:`transform_direction(direction) -> Vector3`

      Treats ``direction`` as ``(x, y, z, 0)``, multiplies it by the matrix and
      returns a :class:`Vector3` without performing a perspective divide.
      Translation therefore does not affect the result.

      .. ## Matrix4x4.transform_direction ##

   .. method:: translate

      | :sl:`returns a new matrix with a translation applied (local frame).`
      | :sg:`translate(offset) -> Matrix4x4`

      Returns a new matrix equal to ``self`` post-multiplied by a translation of
      ``offset`` (a :class:`Vector3` or a sequence of three numbers); ``self``
      is not modified. Post-multiplication means the translation is applied in
      the matrix's own (local) frame. Build a standalone translation with
      ``Matrix4x4().translate(offset)``.

      .. ## Matrix4x4.translate ##

   .. method:: translate_ip

      | :sl:`applies a translation in place (local frame).`
      | :sg:`translate_ip(offset) -> None`

      In-place form of :meth:`translate`; mutates ``self`` and returns ``None``.

      .. ## Matrix4x4.translate_ip ##

   .. method:: rotate

      | :sl:`returns a new matrix with a rotation applied (local frame).`
      | :sg:`rotate(axis, angle) -> Matrix4x4`

      Returns a new matrix equal to ``self`` post-multiplied by a rotation of
      ``angle`` degrees about ``axis`` (a :class:`Vector3` or a sequence of
      three numbers, normalized internally); ``self`` is not modified. In this
      left-handed convention a positive angle is a **clockwise** rotation when
      viewed from the ``+axis`` looking toward the origin. A zero-length axis
      raises a ``ValueError``. Build a standalone rotation with
      ``Matrix4x4().rotate(axis, angle)``.

      .. ## Matrix4x4.rotate ##

   .. method:: rotate_ip

      | :sl:`applies a rotation in place (local frame).`
      | :sg:`rotate_ip(axis, angle) -> None`

      In-place form of :meth:`rotate`; mutates ``self`` and returns ``None``.

      .. ## Matrix4x4.rotate_ip ##

   .. method:: scale

      | :sl:`returns a new matrix with a scale applied (local frame).`
      | :sg:`scale(scalar) -> Matrix4x4`
      | :sg:`scale(vector3) -> Matrix4x4`

      Returns a new matrix equal to ``self`` post-multiplied by a scale;
      ``self`` is not modified. A single number produces a uniform scale; a
      :class:`Vector3` or a sequence of three numbers produces a non-uniform
      scale along the ``x``, ``y`` and ``z`` axes. Build a standalone scale
      with ``Matrix4x4().scale(factor)``.

      .. ## Matrix4x4.scale ##

   .. method:: scale_ip

      | :sl:`applies a scale in place (local frame).`
      | :sg:`scale_ip(scalar) -> None`
      | :sg:`scale_ip(vector3) -> None`

      In-place form of :meth:`scale`; mutates ``self`` and returns ``None``.

      .. ## Matrix4x4.scale_ip ##

   .. method:: transpose

      | :sl:`returns a new transposed matrix.`
      | :sg:`transpose() -> Matrix4x4`

      Returns a new matrix that is the transpose of ``self``; ``self`` is not
      modified.

      .. ## Matrix4x4.transpose ##

   .. method:: transpose_ip

      | :sl:`transposes the matrix in place.`
      | :sg:`transpose_ip() -> None`

      .. ## Matrix4x4.transpose_ip ##

   .. method:: invert

      | :sl:`returns a new inverse matrix.`
      | :sg:`invert() -> Matrix4x4`

      Returns a new matrix that is the inverse of ``self``; ``self`` is not
      modified. A matrix is treated as singular only when its determinant is
      exactly zero (or the computed inverse is not finite), in which case a
      ``ValueError`` is raised. A merely ill-conditioned matrix with a very
      small but non-zero determinant is inverted normally, so legitimately
      small-scale transforms remain invertible.

      .. ## Matrix4x4.invert ##

   .. method:: invert_ip

      | :sl:`inverts the matrix in place.`
      | :sg:`invert_ip() -> None`

      Inverts ``self`` in place. A singular matrix raises a ``ValueError`` and
      leaves ``self`` unchanged.

      .. ## Matrix4x4.invert_ip ##

   .. method:: get_translation

      | :sl:`returns the translation component as a Vector3.`
      | :sg:`get_translation() -> Vector3`

      Returns ``Vector3(m[0,3], m[1,3], m[2,3])``.

      .. ## Matrix4x4.get_translation ##

   .. method:: get_row

      | :sl:`returns a row as a Vector4.`
      | :sg:`get_row(index) -> Vector4`

      Returns row ``index`` (``0..3``) as an independent :class:`Vector4`.

      .. ## Matrix4x4.get_row ##

   .. method:: get_column

      | :sl:`returns a column as a Vector4.`
      | :sg:`get_column(index) -> Vector4`

      Returns column ``index`` (``0..3``) as an independent :class:`Vector4`.

      .. ## Matrix4x4.get_column ##

   .. method:: to_tuple

      | :sl:`returns the 16 elements as a flat tuple.`
      | :sg:`to_tuple() -> tuple`

      Returns the elements in row-major order.

      .. ## Matrix4x4.to_tuple ##

   .. method:: to_list

      | :sl:`returns the 16 elements as a flat list.`
      | :sg:`to_list() -> list`

      Returns the elements in row-major order.

      .. ## Matrix4x4.to_list ##

   .. method:: equals

      | :sl:`compares two matrices with an explicit tolerance.`
      | :sg:`equals(other, tolerance=...) -> bool`

      Returns ``True`` when every element of ``self`` and ``other`` differs by
      no more than ``tolerance``. ``other`` may be a ``Matrix4x4`` or a
      sequence of 16 numbers. If ``tolerance`` is omitted, :attr:`epsilon` is
      used.

      .. ## Matrix4x4.equals ##

   .. method:: copy

      | :sl:`returns a copy of the matrix.`
      | :sg:`copy() -> Matrix4x4`

      .. ## Matrix4x4.copy ##

   .. ## pygame.math.Matrix4x4 ##

.. ## pygame.math ##
