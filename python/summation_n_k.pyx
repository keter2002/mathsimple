'''
    This program generates the formulas to summation of naturals from 0 up to n
    elevated in k.
    Copyright (C) 2025  João Manica  <joaoedisonmanica@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
'''

from numpy.polynomial import Polynomial


for k in range(7):
    f_t = Polynomial([1 if i == k else 0 for i in range(k+1)])

    # If f(t) = t^n ->
    T = [[float(f_t(0)), float(f_t(1))],
         [0, float(f_t(1))-float(f_t(0))]]
    i = 1
    while T[i][i] != 0:
        i += 1
        T[0].append(float(f_t(i)))
        for j in range(1, i):
            T[j].append(T[j-1][i] - T[j-1][i-1])
        T.append([0 for j in range(i)])
        T[i].append(T[i-1][i] - T[i-1][i-1])

    coefficients = [T[j][j] for j in range(i)]
    f_np1 = Polynomial([1], symbol='n')
    f_n = Polynomial([0], symbol='n')
    for j in range(i):
        f_np1 *= Polynomial([1-j, 1], symbol='n')/(j+1)
        f_n += coefficients[j]*f_np1
    print(f'Summation of 0^{k} + 1^{k}+ ... + n^{k}')
    print(f_n)
