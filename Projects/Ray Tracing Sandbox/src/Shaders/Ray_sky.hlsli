float3 SkyTrace(float3 ray){
    float3 sunRay = float3(1.0f, 2.0f, 1.0f);
    sunRay = normalize(sunRay);
    ray = normalize(ray);
    float dist = distance(ray, sunRay);

    float3 bright = float3(2.3f, 2.5f, 1.1f);
    float3 dark = float3(0.2f, 0.45f, 0.65f);
    
    return dark + (1.0f / (1.0f + 4.0f * dist)) * bright;
}