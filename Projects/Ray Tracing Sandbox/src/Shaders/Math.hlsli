void Rotate(in out float3 p, float2 a) {
    [unroll(2)] for (uint i = 0; i < 2; i++) {
        const float c = cos(a[i]);
        const float s = sin(a[i]);
        float x = p.x * c - p[i + 1] * s;
        float y = p[i + 1] * c + p.x * s;
        p.x = x;
        p[i + 1] = y;
    }
}

