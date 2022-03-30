void main() {
    vec4 p = topaz_mat_projection * (topaz_mat_modelview * (vec4(topaz_vertex_position, 1.0)));
    topazSetPosition(p);
    topazSetUV(topaz_vertex_uv);
}


