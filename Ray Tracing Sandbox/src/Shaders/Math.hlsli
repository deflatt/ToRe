#define ll int64_t
#define ll2 int64_t2
#define ll3 int64_t3

static const ll numDecimals = 16;
static const ll intRatio = ((ll)1 << numDecimals);
static const float floatRatio = 1.0f / (float)intRatio;
static const ll inf = ((ll)1 << 62);

ll ToInt(float a) {
   return (ll)(a * (float) intRatio);
}
ll ToFloat(ll a) {
    return (float)a * floatRatio;
}

ll iMul(ll a, ll b) {
    return ll(((ll) a * (ll) b) >> numDecimals);
}
ll iDiv(ll a, ll b) {
    if (b == 0LL) {
        if (a == 0)
            return intRatio;
        else if (a > 0LL)
            return inf;
        else
            return -inf;
    }
    return ll(((ll)a << numDecimals) / (ll)b);
}
ll3 iMul(ll3 a, ll3 b) {
    return ll3(iMul(a.x, b.x), iMul(a.y, b.y), iMul(a.z, b.z));
}
ll3 iDiv(ll3 a, ll3 b) {
    return ll3(iDiv(a.x, b.x), iDiv(a.y, b.y), iDiv(a.z, b.z));
}

ll iCos(ll a){
    return ll(cos((float) a * floatRatio) * (float) intRatio);
}
ll iSin(ll a){
    return ll(sin((float) a * floatRatio) * (float) intRatio);
}
ll iTan(ll a){
    return ll(tan((float) a * floatRatio) * (float) intRatio);
}

void iRotate(in out ll3 p, ll2 a) {
    [unroll(2)] for (uint i = 0; i < 2; i++) {
        const ll c = iCos(a[i]);
        const ll s = iSin(a[i]);
        ll x = p.x * c - p[i + 1] * s;
        ll y = p[i + 1] * c + p.x * s;
        p.x = x;
        p[i + 1] = y;
    }
}