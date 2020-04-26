# Triangulation in ℝ³:
Assume we have a point P representing a human, and two wifi detectors A, and B.\
Let the signal strength recieved from P to A, and from P to B be As, and Bs\
Then de distance dA = sqrt(As) and dB = sqrt(Bs) as the strength falls with the square of the distance.\
With only dA and dB there exists multiple possible positions for P.

To get these positions we can form two lines with length dA, and dB, if we let P be (0, 0, 0) then the vector
![formula](https://render.githubusercontent.com/render/math?math=\vec{A}) and the vector ![formula](https://render.githubusercontent.com/render/math?math=\vec{B}) would be possible in any arbitrary direction since we have no correalations between A, and B.

To be able to triangulate only one position in ℝ³ we therefore require a third sensor. Let this sensor be denoted by C.

# Approximation of P
The distance dA, dB, and dC forms three spheres. The possible position of P is the intersection of these spheres.\
If there are no intersections between all the spheres then the distance of the one not intersecting is from another device.\
If all three spheres intersect then we form a volume Pv in which we let the middle represent our approximatory position P.

# In practice
In practice the ESPs RSSI maxes out at 100 dB. To make things make more sence we will calculate the distances by:\
dX = sqrt(100 + Xs) for every module X, Xs is in the range (-100, 0] and we transformed this to [0, 100).\
For triangulation we will also assume that each module has roughly the same z coordinate.\
Otherwise we will have to do sphere-sphere-sphere intersection which requires a lot of computation.\
With the assumptions that the position of A, B, C is known then their distance to P(0, 0, 0) will instead form three circles, Ca, Cb, and Cc.
Using Triangulation for this project might therefor not be the way to go.

# Trilateration
Trilateration is more cost effective than triangulation considering limited components and the relatively slow instruction per second.\
Trilateration has two special variables n and Ptx, n is a path-loss exponent which is environment specific. For indoors areas this n is close to 1.8\
Ptx marks the Default transmit power and this is the variable that provides most error, this is dependent on manufacturer and varies from module to module.

RSSI = Ptx - 10 * n * log10(ESP_distance)\
ESP_distance = 10^((Ptx - RSSI) / (10 * n)) 

# Trilateration given three points
here is pseudo code for trilateration on the points A, B, C and distances dA, dB, dC
```js
function trilateration(A, B, C, dA, dB, dC) -> Point {

    let ex = vecDivide(vecSub(B, A), norm(vecSub(B, A)))
    let i = dot(ex, vecSub(C, A))
    let a = vecSub(vecSub(C, A), vecMult(ex, i))
    let ey = vecDiv(a, norm(a))
    let ez = vecCross(ex, ey)
    let d = norm(vecSub(B, A))
    let j = dot(ey, vecSub(C, A))
    
    let x = (sqrt(dA) - sqrt(dB) + sqrt(dC)) / (2*d)
    let y = (sqrt(dA) - sqrt(dC) + sqrt(i) + sqrt(j)) / (2*j) - (i/j) * x

    P = vecAdd(A, vecAdd(vecMult(ex, x), vecMult(ey, y)))
    return P
}
```
There is a possiblility that there exists 2 cases that have different z value, we get the mean value between the two.