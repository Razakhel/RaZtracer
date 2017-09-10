template <typename T, unsigned int Size>
Vector<T, Size>::Vector(const std::initializer_list<T>& list) {
  auto element = list.begin();

  for (std::size_t i = 0; i < list.size(); ++i, ++element)
    data[i] = *element;
}

template <typename T, unsigned int Size>
T Vector<T, Size>::dot(const Vector<T, Size>& vec) const {
  float res = 0.f;
  for (std::size_t i = 0; i < data.size(); ++i)
    res += data[i] * vec.getData()[i];
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator+(Vector<T, Size> vec) {
  vec += *this;
  return vec;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator+(float val) {
  Vector<T, Size> res = *this;
  res += val;
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator-(Vector<T, Size> vec) {
  vec -= *this;
  return vec;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator-(float val) {
  Vector<T, Size> res = *this;
  res -= val;
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator*(Vector<T, Size> vec) {
  vec *= *this;
  return vec;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator*(float val) {
  Vector<T, Size> res = *this;
  res *= val;
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator/(Vector<T, Size> vec) {
  vec /= *this;
  return vec;
}

template <typename T, unsigned int Size>
Vector<T, Size> Vector<T, Size>::operator/(float val) {
  Vector<T, Size> res = *this;
  res /= val;
  return res;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator+=(const Vector<T, Size>& vec) {
  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] += vec.getData()[i];
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator+=(float val) {
  for (T& it : data)
    it += val;
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator-=(const Vector<T, Size>& vec) {
  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] -= vec.getData()[i];
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator-=(float val) {
  for (T& it : data)
    it -= val;
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator*=(const Vector<T, Size>& vec) {
  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] *= vec.getData()[i];
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator*=(float val) {
  for (T& it : data)
    it *= val;
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator/=(const Vector<T, Size>& vec) {
  for (std::size_t i = 0; i < data.size(); ++i)
    data[i] /= vec.getData()[i];
  return *this;
}

template <typename T, unsigned int Size>
Vector<T, Size>& Vector<T, Size>::operator/=(float val) {
  for (T& it : data)
    it /= val;
  return *this;
}
