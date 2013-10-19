#ifndef strong_gl_enums_hpp_renderstack_toolkit
#define strong_gl_enums_hpp_renderstack_toolkit

namespace gl {

namespace active_attrib_type
{
    enum value
    {
        int_ = ((int)0x1404),
        unsigned_int = ((int)0x1405),
        float_ = ((int)0x1406),
        double_ = ((int)0x140a),
        float_vec2 = ((int)0x8b50),
        float_vec3 = ((int)0x8b51),
        float_vec4 = ((int)0x8b52),
        int_vec2 = ((int)0x8b53),
        int_vec3 = ((int)0x8b54),
        int_vec4 = ((int)0x8b55),
        float_mat2 = ((int)0x8b5a),
        float_mat3 = ((int)0x8b5b),
        float_mat4 = ((int)0x8b5c),
        unsigned_int_vec2 = ((int)0x8dc6),
        unsigned_int_vec3 = ((int)0x8dc7),
        unsigned_int_vec4 = ((int)0x8dc8),
        double_mat2 = ((int)0x8f46),
        double_mat3 = ((int)0x8f47),
        double_mat4 = ((int)0x8f48),
        double_mat2x3 = ((int)0x8f49),
        double_mat2x4 = ((int)0x8f4a),
        double_mat3x2 = ((int)0x8f4b),
        double_mat3x4 = ((int)0x8f4c),
        double_mat4x2 = ((int)0x8f4d),
        double_mat4x3 = ((int)0x8f4e),
        double_vec2 = ((int)0x8ffc),
        double_vec3 = ((int)0x8ffd),
        double_vec4 = ((int)0x8ffe),
    };
}
namespace active_uniform_block_parameter
{
    enum value
    {
        uniform_block_referenced_by_tess_control_shader = ((int)0x84F0),
        uniform_block_referenced_by_tess_evaluation_shader = ((int)0x84F1),
        uniform_block_binding = ((int)0x8A3F),
        uniform_block_data_size = ((int)0x8A40),
        uniform_block_name_length = ((int)0x8A41),
        uniform_block_active_uniforms = ((int)0x8A42),
        uniform_block_active_uniform_indices = ((int)0x8A43),
        uniform_block_referenced_by_vertex_shader = ((int)0x8A44),
        uniform_block_referenced_by_fragment_shader = ((int)0x8A46),
    };
}
namespace active_uniform_type 
{
    enum value
    {
        int_ = ((int)0x1404),
        unsigned_int = ((int)0x1405),
        float_ = ((int)0x1406),
        double_ = ((int)0x140A),
        float_vec2 = ((int)0x8B50),
        float_vec3 = ((int)0x8B51),
        float_vec4 = ((int)0x8B52),
        int_vec2 = ((int)0x8B53),
        int_vec3 = ((int)0x8B54),
        int_vec4 = ((int)0x8B55),
        bool_ = ((int)0x8B56),
        bool_vec2 = ((int)0x8B57),
        bool_vec3 = ((int)0x8B58),
        bool_vec4 = ((int)0x8B59),
        float_mat2 = ((int)0x8B5A),
        float_mat3 = ((int)0x8B5B),
        float_mat4 = ((int)0x8B5C),
        sampler_1d = ((int)0x8B5D),
        sampler_2d = ((int)0x8B5E),
        sampler_3d = ((int)0x8B5F),
        sampler_cube = ((int)0x8B60),
        sampler_1d_shadow = ((int)0x8B61),
        sampler_2d_shadow = ((int)0x8B62),
        sampler_2d_rect = ((int)0x8B63),
        sampler_2d_rect_shadow = ((int)0x8B64),
        float_mat_2x3 = ((int)0x8B65),
        float_mat_2x4 = ((int)0x8B66),
        float_mat_3x2 = ((int)0x8B67),
        float_mat_3x4 = ((int)0x8B68),
        float_mat_4x2 = ((int)0x8B69),
        float_mat_4x3 = ((int)0x8B6A),
        sampler_1d_array = ((int)0x8DC0),
        sampler_2d_array = ((int)0x8DC1),
        sampler_buffer = ((int)0x8DC2),
        sampler_1d_array_shadow = ((int)0x8DC3),
        sampler_2d_array_shadow = ((int)0x8DC4),
        sampler_cube_shadow = ((int)0x8DC5),
        unsigned_int_vec2 = ((int)0x8DC6),
        unsigned_int_vec3 = ((int)0x8DC7),
        unsigned_int_vec4 = ((int)0x8DC8),
        int_sampler_1d = ((int)0x8DC9),
        int_sampler_2d = ((int)0x8DCA),
        int_sampler_3d = ((int)0x8DCB),
        int_sampler_cube = ((int)0x8DCC),
        int_sampler_2d_rect = ((int)0x8DCD),
        int_sampler_1d_array = ((int)0x8DCE),
        int_sampler_2d_array = ((int)0x8DCF),
        int_sampler_buffer = ((int)0x8DD0),
        unsigned_int_sampler_1d = ((int)0x8DD1),
        unsigned_int_sampler_2d = ((int)0x8DD2),
        unsigned_int_sampler_3d = ((int)0x8DD3),
        unsigned_int_sampler_cube = ((int)0x8DD4),
        unsigned_int_sampler_2d_rect = ((int)0x8DD5),
        unsigned_int_sampler_1d_array = ((int)0x8DD6),
        unsigned_int_sampler_2d_array = ((int)0x8DD7),
        unsigned_int_sampler_buffer = ((int)0x8DD8),
        double_mat_2 = ((int)0x8F46),
        double_mat_3 = ((int)0x8F47),
        double_mat_4 = ((int)0x8F48),
        double_mat_2x3 = ((int)0x8F49),
        double_mat_2x4 = ((int)0x8F4A),
        double_mat_3x2 = ((int)0x8F4B),
        double_mat_3x4 = ((int)0x8F4C),
        double_mat_4x2 = ((int)0x8F4D),
        double_mat_4x3 = ((int)0x8F4E),
        double_vec2 = ((int)0x8FFC),
        double_vec3 = ((int)0x8FFD),
        double_vec4 = ((int)0x8FFE),
        sampler_cubemap_array = ((int)0x900C),
        sampler_cubemap_array_shadow = ((int)0x900D),
        int_sampler_cubemap_array = ((int)0x900E),
        unsigned_int_sampler_cubemap_array = ((int)0x900F),
        sampler_2d_multisample = ((int)0x9108),
        int_sampler_2d_multisample = ((int)0x9109),
        unsigned_int_sampler_2d_multisample = ((int)0x910A),
        sampler_2d_multisample_array = ((int)0x910B),
        int_sampler_2d_multisample_array = ((int)0x910C),
        unsigned_int_sampler_2d_multisample_array = ((int)0x910D),
    };
}
namespace array_cap
{
    enum value
    {
        vertex_array = ((int)0x8074),
        normal_array = ((int)0x8075),
        color_array = ((int)0x8076),
        index_array = ((int)0x8077),
        texture_coord_array = ((int)0x8078),
        edge_flag_array = ((int)0x8079),
        fog_coord_array = ((int)0x8457),
        secondary_color_array = ((int)0x845E),
    };
}
namespace begin_feedback_mode
{
    enum value 
    {
        points = ((int)0x0000),
        lines = ((int)0x0001),
        triangles = ((int)0x0004),
    };
}
namespace begin_mode
{
    enum value
    {
        points = ((int)0x0000),
        lines = ((int)0x0001),
        line_loop = ((int)0x0002),
        line_strip = ((int)0x0003),
        triangles = ((int)0x0004),
        triangle_strip = ((int)0x0005),
        triangle_fan = ((int)0x0006),
#if defined(SUPPORT_LEGACY_OPENGL)
        quads = ((int)0x0007),
        quad_strip = ((int)0x0008),
        polygon = ((int)0x0009),
#endif
        patches = ((int)0x000E),
        lines_adjacency = ((int)0xA),
        line_strip_adjacency = ((int)0xB),
        triangles_adjacency = ((int)0xC),
        triangle_strip_adjacency = ((int)0xD),
    };
}
namespace blend_equation_mode
{
    enum value 
    {
        func_add = ((int)0x8006),
        min = ((int)0x8007),
        max = ((int)0x8008),
        func_subtract = ((int)0x800A),
        func_reverse_subtract = ((int)0x800B),
    };
}
namespace blending_factor_dest
{
    enum value
    {
        zero = ((int)0),
        src_color = ((int)0x0300),
        one_minus_src_color = ((int)0x0301),
        src_alpha = ((int)0x0302),
        one_minus_src_alpha = ((int)0x0303),
        dst_alpha = ((int)0x0304),
        one_minus_dst_alpha = ((int)0x0305),
        constant_color = ((int)0x8001),
        one_minus_constant_color = ((int)0x8002),
        constant_alpha = ((int)0x8003),
        one_minus_constant_alpha = ((int)0x8004),
        src1_alpha = ((int)0x8589),
        src1_color = ((int)0x88F9),
        one_minus_src1_color = ((int)0x88FA),
        one_minus_src1_alpha = ((int)0x88FB),
        one = ((int)1),
    };
}
namespace blending_factor_src
{
    enum value
    {
        zero = ((int)0),
        src_alpha = ((int)0x0302),
        one_minus_src_alpha = ((int)0x0303),
        dst_alpha = ((int)0x0304),
        one_minus_dst_alpha = ((int)0x0305),
        dst_color = ((int)0x0306),
        one_minus_dst_color = ((int)0x0307),
        src_alpha_saturate = ((int)0x0308),
        constant_color = ((int)0x8001),
        one_minus_constant_color = ((int)0x8002),
        constant_alpha = ((int)0x8003),
        one_minus_constant_alpha = ((int)0x8004),
        src1_alpha = ((int)0x8589),
        src1_color = ((int)0x88F9),
        one_minus_src1_color = ((int)0x88FA),
        one_minus_src1_alpha = ((int)0x88FB),
        one = ((int)1),
    };
}
namespace blit_framebuffer_filter
{
    enum value
    {
        nearest = ((int)0x2600),
        linear = ((int)0x2601),
    };
}
namespace buffer_access
{
    enum value
    {
        read_only = ((int)0x88B8),
        write_only = ((int)0x88B9),
        read_write = ((int)0x88BA),
    };
}
namespace buffer_access_mask
{
    enum value
    {
        map_read_bit = ((int)0x0001),
        map_write_bit = ((int)0x0002),
        map_invalidate_range_bit = ((int)0x0004),
        map_invalidate_buffer_bit = ((int)0x0008),
        map_flush_explicit_bit = ((int)0x0010),
        map_unsynchronized_bit = ((int)0x0020),
    };
}
namespace buffer_target
{
    enum value
    {
        array_buffer = ((int)0x8892),
        element_array_buffer = ((int)0x8893),
        pixel_pack_buffer = ((int)0x88EB),
        pixel_unpack_buffer = ((int)0x88EC),
        uniform_buffer = ((int)0x8A11),
        texture_buffer = ((int)0x8C2A),
        transform_feedback_buffer = ((int)0x8C8E),
        copy_read_buffer = ((int)0x8F36),
        copy_write_buffer = ((int)0x8F37),
        draw_indirect_buffer = ((int)0x8F3F),
    };
}
namespace buffer_usage_hint
{
    enum value
    {
        stream_draw = ((int)0x88E0),
        stream_read = ((int)0x88E1),
        stream_copy = ((int)0x88E2),
        static_draw = ((int)0x88E4),
        static_read = ((int)0x88E5),
        static_copy = ((int)0x88E6),
        dynamic_draw = ((int)0x88E8),
        dynamic_read = ((int)0x88E9),
        dynamic_copy = ((int)0x88EA)
    };
}
namespace clear_buffer_mask
{
    enum value
    {
        depth_buffer_bit = ((int)0x00000100),
        accum_buffer_bit = ((int)0x00000200),
        stencil_buffer_bit = ((int)0x00000400),
        color_buffer_bit = ((int)0x00004000),
        coverage_buffer_bit = ((int)0x00008000),
    };
}
namespace cull_face_mode
{
    enum value
    {
        front = ((int)0x0404),
        back = ((int)0x0405),
        front_and_back = ((int)0x0408),
    };
}
namespace depth_function
{
    enum value
    {
        never = ((int)0x0200),
        less = ((int)0x0201),
        equal = ((int)0x0202),
        l_equal = ((int)0x0203),
        greater = ((int)0x0204),
        not_equal = ((int)0x0205),
        g_equal = ((int)0x0206),
        always = ((int)0x0207),
    };
}
namespace draw_elements_type
{
    enum value
    {
        unsigned_byte = ((int)0x1401),
        unsigned_short = ((int)0x1403),
        unsigned_int = ((int)0x1405),
    };
}
namespace enable_cap
{
    enum value
    {
        point_smooth = ((int)0x0B10),
        line_smooth = ((int)0x0B20),
        line_stipple = ((int)0x0B24),
        polygon_smooth = ((int)0x0B41),
        polygon_stipple = ((int)0x0B42),
        cull_face = ((int)0x0B44),
#if defined(SUPPORT_LEGACY_OPENGL)
        lighting = ((int)0x0B50),
        color_material = ((int)0x0B57),
        fog = ((int)0x0B60),
#endif
        depth_test = ((int)0x0B71),
        stencil_test = ((int)0x0B90),
#if defined(SUPPORT_LEGACY_OPENGL)
        normalize = ((int)0x0BA1),
        alpha_test = ((int)0x0BC0),
#endif
        dither = ((int)0x0BD0),
        blend = ((int)0x0BE2),
        index_logic_op = ((int)0x0BF1),
        color_logic_op = ((int)0x0BF2),
        scissor_test = ((int)0x0C11),
#if defined(SUPPORT_LEGACY_OPENGL)
        texture_gen_s = ((int)0x0C60),
        texture_gen_t = ((int)0x0C61),
        texture_gen_r = ((int)0x0C62),
        texture_gen_q = ((int)0x0C63),
#endif
        auto_normal = ((int)0x0D80),
        /*map1_color4 = ((int)0x0D90),
        map1_index = ((int)0x0D91),
        map1_normal = ((int)0x0D92),
        map1_texture_coord1 = ((int)0x0D93),
        map1_texture_coord2 = ((int)0x0D94),
        map1_texture_coord3 = ((int)0x0D95),
        map1_texture_coord4 = ((int)0x0D96),
        map1_vertex3 = ((int)0x0D97),
        map1_vertex4 = ((int)0x0D98),
        map2_color4 = ((int)0x0DB0),
        map2_index = ((int)0x0DB1),
        map2_normal = ((int)0x0DB2),
        map2_texture_coord1 = ((int)0x0DB3),
        map2_texture_coord2 = ((int)0x0DB4),
        map2_texture_coord3 = ((int)0x0DB5),
        map2_texture_coord4 = ((int)0x0DB6),
        map2_vertex3 = ((int)0x0DB7),
        map2_vertex4 = ((int)0x0DB8),/*/
        texture_1d = ((int)0x0DE0),
        texture_2d = ((int)0x0DE1),
        polygon_offset_point = ((int)0x2A01),
        polygon_offset_line = ((int)0x2A02),
#if defined(SUPPORT_LEGACY_OPENGL)
        clip_plane0 = ((int)0x3000),
        clip_plane1 = ((int)0x3001),
        clip_plane2 = ((int)0x3002),
        clip_plane3 = ((int)0x3003),
        clip_plane4 = ((int)0x3004),
        clip_plane5 = ((int)0x3005),
        light0 = ((int)0x4000),
        light1 = ((int)0x4001),
        light2 = ((int)0x4002),
        light3 = ((int)0x4003),
        light4 = ((int)0x4004),
        light5 = ((int)0x4005),
        light6 = ((int)0x4006),
        light7 = ((int)0x4007),
#endif
        /*convolution_1d = ((int)0x8010),
        convolution_2d = ((int)0x8011),
        separable_2d = ((int)0x8012),
        histogram = ((int)0x8024),*/
        polygon_offset_fill = ((int)0x8037),
#if defined(SUPPORT_LEGACY_OPENGL)
        rescale_normal = ((int)0x803A),
        vertex_array = ((int)0x8074),
        normal_array = ((int)0x8075),
        color_array = ((int)0x8076),
        index_array = ((int)0x8077),
        texture_coord_array = ((int)0x8078),
        edge_flag_array = ((int)0x8079),
#endif
        multisample = ((int)0x809D),
        sample_alpha_to_coverage = ((int)0x809E),
        sample_alpha_to_one = ((int)0x809F),
        sample_coverage = ((int)0x80A0),
        //color_table = ((int)0x80D0),
        //post_convolution_color_table = ((int)0x80D1),
        //post_color_matrix_color_table = ((int)0x80D2),
#if defined(SUPPORT_LEGACY_OPENGL)
        fog_coord_array = ((int)0x8457),
#endif
        color_sum = ((int)0x8458),
#if defined(SUPPORT_LEGACY_OPENGL)
        secondary_color_array = ((int)0x845E),
#endif
        texture_rectangle = ((int)0x84F5),
        texture_cube_map = ((int)0x8513),
        program_point_size = ((int)0x8642),
        vertex_program_point_size = ((int)0x8642),
        vertex_program_two_side = ((int)0x8643),
        depth_clamp = ((int)0x864F),
        texture_cube_map_seamless = ((int)0x884F),
        point_sprite = ((int)0x8861),
        sample_shading = ((int)0x8C36),
        rasterizer_discard = ((int)0x8C89),
        framebuffer_srgb = ((int)0x8DB9),
        sample_mask = ((int)0x8E51),
        primitive_restart = ((int)0x8F9D),
    };
}
namespace framebuffer_attachment
{
    enum value
    {
        depth_stencil_attachment = ((int)0x821A),
        color_attachment_0 = ((int)0x8CE0),
        color_attachment_1 = ((int)0x8CE1),
        color_attachment_2 = ((int)0x8CE2),
        color_attachment_3 = ((int)0x8CE3),
        color_attachment_4 = ((int)0x8CE4),
        color_attachment_5 = ((int)0x8CE5),
        color_attachment_6 = ((int)0x8CE6),
        color_attachment_7 = ((int)0x8CE7),
        color_attachment_8 = ((int)0x8CE8),
        color_attachment_9 = ((int)0x8CE9),
        color_attachment_10 = ((int)0x8CEA),
        color_attachment_11 = ((int)0x8CEB),
        color_attachment_12 = ((int)0x8CEC),
        color_attachment_13 = ((int)0x8CED),
        color_attachment_14 = ((int)0x8CEE),
        color_attachment_15 = ((int)0x8CEF),
        depth_attachment = ((int)0x8D00),
        stencil_attachment = ((int)0x8D20),
    };
}
namespace framebuffer_error_code //framebuffer_error_code
{
    enum value
    {
        framebuffer_undefined = ((int)0x8219),
        framebuffer_complete = ((int)0x8CD5),
        framebuffer_incomplete_attachment = ((int)0x8CD6),
        framebuffer_incomplete_missing_attachment = ((int)0x8CD7),
        framebuffer_incomplete_draw_buffer = ((int)0x8CDB),
        framebuffer_incomplete_read_buffer = ((int)0x8CDC),
        framebuffer_unsupported = ((int)0x8CDD),
        framebuffer_incomplete_multisample = ((int)0x8D56),
        framebuffer_incomplete_layer_targets = ((int)0x8DA8),
        framebuffer_incomplete_layer_count = ((int)0x8DA9),
    };
}
namespace framebuffer_target
{
    enum value
    {
        read_framebuffer = ((int)0x8CA8),
        draw_framebuffer = ((int)0x8CA9),
        framebuffer = ((int)0x8D40),
    };
}
namespace front_face_direction
{
    enum value
    {
        cw = ((int)0x0900),
        ccw = ((int)0x0901),
    };
}
namespace get_query_object_param
{
    enum value
    {
        query_result = ((int)0x8866),
        query_result_available = ((int)0x8867),
    };
}
namespace get_query_param
{
    enum value
    {
        query_counter_bits = ((int)0x8864),
        current_query = ((int)0x8865),
    };
}
namespace patch_parameter_float
{
    enum value
    {
        patch_default_inner_level = ((int)0x8E73),
        patch_default_outer_level = ((int)0x8E74),
    };
}
namespace patch_parameter_int
{
    enum value
    {
        patch_vertices = ((int)0x8E72),
    };
}
namespace pixel_format
{
    enum value
    {
        invalid = 0,
        color_index = ((int)0x1900),
        stencil_index = ((int)0x1901),
        depth_component = ((int)0x1902),
        red = ((int)0x1903),
        green = ((int)0x1904),
        blue = ((int)0x1905),
        alpha = ((int)0x1906),
        rgb = ((int)0x1907),
        rgba = ((int)0x1908),
        luminance = ((int)0x1909),
        luminance_alpha = ((int)0x190A),
        bgr = ((int)0x80E0),
        bgra = ((int)0x80E1),
        rg = ((int)0x8227),
        rg_integer = ((int)0x8228),
        depth_stencil = ((int)0x84F9),
        red_integer = ((int)0x8D94),
        green_integer = ((int)0x8D95),
        blue_integer = ((int)0x8D96),
        alpha_integer = ((int)0x8D97),
        rgb_integer = ((int)0x8D98),
        rgba_integer = ((int)0x8D99),
        bgr_integer = ((int)0x8D9A),
        bgra_integer = ((int)0x8D9B),
    };
}
namespace pixel_internal_format
{
    enum value
    {
        invalid = 0,
        depth_component = ((int)0x1902),
        alpha = ((int)0x1906),
        rgb = ((int)0x1907),
        rgba = ((int)0x1908),
        luminance = ((int)0x1909),
        luminance_alpha = ((int)0x190A),
        r3_g3_b2 = ((int)0x2A10),
        alpha4 = ((int)0x803B),
        alpha8 = ((int)0x803C),
        alpha12 = ((int)0x803D),
        alpha16 = ((int)0x803E),
        luminance4 = ((int)0x803F),
        luminance8 = ((int)0x8040),
        luminance12 = ((int)0x8041),
        luminance16 = ((int)0x8042),
        luminance4_alpha4 = ((int)0x8043),
        luminance6_alpha2 = ((int)0x8044),
        luminance8_alpha8 = ((int)0x8045),
        luminance12_alpha4 = ((int)0x8046),
        luminance12_alpha12 = ((int)0x8047),
        luminance16_alpha16 = ((int)0x8048),
        intensity = ((int)0x8049),
        intensity4 = ((int)0x804A),
        intensity8 = ((int)0x804B),
        intensity12 = ((int)0x804C),
        intensity16 = ((int)0x804D),
        rgb4 = ((int)0x804F),
        rgb5 = ((int)0x8050),
        rgb8 = ((int)0x8051),
        rgb10 = ((int)0x8052),
        rgb12 = ((int)0x8053),
        rgb16 = ((int)0x8054),
        rgba2 = ((int)0x8055),
        rgba4 = ((int)0x8056),
        rgb5_a1 = ((int)0x8057),
        rgba8 = ((int)0x8058),
        rgb10_a2 = ((int)0x8059),
        rgba12 = ((int)0x805A),
        rgba16 = ((int)0x805B),
        depth_component16 = ((int)0x81a5),
        depth_component24 = ((int)0x81a6),
        depth_component32 = ((int)0x81a7),
        compressed_red = ((int)0x8225),
        compressed_rg = ((int)0x8226),
        r8 = ((int)0x8229),
        r16 = ((int)0x822A),
        rg8 = ((int)0x822B),
        rg16 = ((int)0x822C),
        r16f = ((int)0x822D),
        r32f = ((int)0x822E),
        rg16f = ((int)0x822F),
        rg32f = ((int)0x8230),
        r8i = ((int)0x8231),
        r8ui = ((int)0x8232),
        r16i = ((int)0x8233),
        r16ui = ((int)0x8234),
        r32i = ((int)0x8235),
        r32ui = ((int)0x8236),
        rg8i = ((int)0x8237),
        rg8ui = ((int)0x8238),
        rg16i = ((int)0x8239),
        rg16ui = ((int)0x823A),
        rg32i = ((int)0x823B),
        rg32ui = ((int)0x823C),
        compressed_alpha = ((int)0x84E9),
        compressed_luminance = ((int)0x84EA),
        compressed_luminance_alpha = ((int)0x84EB),
        compressed_intensity = ((int)0x84EC),
        compressed_rgb = ((int)0x84ED),
        compressed_rgba = ((int)0x84EE),
        depth_stencil = ((int)0x84F9),
        rgba32f = ((int)0x8814),
        rgb32f = ((int)0x8815),
        rgba16f = ((int)0x881A),
        rgb16f = ((int)0x881B),
        depth24_stencil8 = ((int)0x88F0),
        r11f_g11f_b10f = ((int)0x8C3A),
        rgb9_e5 = ((int)0x8C3D),
        srgb = ((int)0x8C40),
        srgb8 = ((int)0x8C41),
        srgb_alpha = ((int)0x8C42),
        srgb8_alpha8 = ((int)0x8C43),
        sluminance_alpha = ((int)0x8C44),
        sluminance8_alpha8 = ((int)0x8C45),
        sluminance = ((int)0x8C46),
        sluminance8 = ((int)0x8C47),
        compressed_srgb = ((int)0x8C48),
        compressed_srgb_alpha = ((int)0x8C49),
        compressed_sluminance = ((int)0x8C4A),
        compressed_sluminance_alpha = ((int)0x8C4B),
        depth_component32f = ((int)0x8CAC),
        depth32f_stencil8 = ((int)0x8CAD),
        rgba32ui = ((int)0x8D70),
        rgb32ui = ((int)0x8D71),
        rgba16ui = ((int)0x8D76),
        rgb16ui = ((int)0x8D77),
        rgba8ui = ((int)0x8D7C),
        rgb8ui = ((int)0x8D7D),
        rgba32i = ((int)0x8D82),
        rgb32i = ((int)0x8D83),
        rgba16i = ((int)0x8D88),
        rgb16i = ((int)0x8D89),
        rgba8i = ((int)0x8D8E),
        rgb8i = ((int)0x8D8F),
        float32_unsigned_int248rev = ((int)0x8DAD),
        compressed_red_rgtc1 = ((int)0x8DBB),
        compressed_signed_red_rgtc1 = ((int)0x8DBC),
        compressed_rg_rgtc2 = ((int)0x8DBD),
        compressed_signed_rg_rgtc2 = ((int)0x8DBE),
        rgb10_a2ui = ((int)0x906F),
        one = ((int)1),
        two = ((int)2),
        three = ((int)3),
        four = ((int)4),
    };
}
namespace pixel_type
{
    enum value
    {
        byte = ((int)0x1400),
        unsigned_byte = ((int)0x1401),
        short_ = ((int)0x1402),
        unsigned_short = ((int)0x1403),
        int_ = ((int)0x1404),
        unsigned_int = ((int)0x1405),
        float_ = ((int)0x1406),
        half_float = ((int)0x140B),
        bitmap = ((int)0x1A00),
        unsigned_byte_332 = ((int)0x8032),
        unsigned_short_4444 = ((int)0x8033),
        unsigned_short_5551 = ((int)0x8034),
        unsigned_int_8888 = ((int)0x8035),
        unsigned_int_1010102 = ((int)0x8036),
        unsigned_byte_233_reversed = ((int)0x8362),
        unsigned_short_565 = ((int)0x8363),
        unsigned_short_565_reversed = ((int)0x8364),
        unsigned_short_4444_reversed = ((int)0x8365),
        unsigned_short_1555_reversed = ((int)0x8366),
        unsigned_int_8888_reversed = ((int)0x8367),
        unsigned_int_2101010Reversed = ((int)0x8368),
        unsigned_int_248 = ((int)0x84FA),
        unsigned_int_10_f11_f11f_rev = ((int)0x8C3B),
        unsigned_int_5999_rev = ((int)0x8C3E),
        float32_unsigned_int_248_rev = ((int)0x8DAD),
    };
}
namespace program_parameter
{
    enum value
    {
        program_binary_retrievable_hint = ((int)0x8257),
        program_separable = ((int)0x8258),
        program_binary_length = ((int)0x8741),
        geometry_shader_invocations = ((int)0x887F),
        active_uniform_block_max_name_length = ((int)0x8A35),
        active_uniform_blocks = ((int)0x8A36),
        delete_status = ((int)0x8B80),
        link_status = ((int)0x8B82),
        validate_status = ((int)0x8B83),
        info_log_length = ((int)0x8B84),
        attached_shaders = ((int)0x8B85),
        active_uniforms = ((int)0x8B86),
        active_uniform_max_length = ((int)0x8B87),
        active_attributes = ((int)0x8B89),
        active_attribute_max_length = ((int)0x8B8A),
        transform_feedback_varying_max_length = ((int)0x8C76),
        transform_feedback_buffer_mode = ((int)0x8C7F),
        transform_feedback_varyings = ((int)0x8C83),
        geometry_vertices_out = ((int)0x8DDA),
        geometry_input_type = ((int)0x8DDB),
        geometry_output_type = ((int)0x8DDC),
        tess_control_output_vertices = ((int)0x8E75),
        tess_gen_mode = ((int)0x8E76),
        tess_gen_spacing = ((int)0x8E77),
        tess_gen_vertex_order = ((int)0x8E78),
        tess_gen_point_mode = ((int)0x8E79),
    };
}
namespace query_target
{
    enum value
    {
        time_elapsed = ((int)0x88BF),
        samples_passed = ((int)0x8914),
        any_samples_passed = ((int)0x8C2F),
        primitives_generated = ((int)0x8C87),
        transform_feedback_primitives_written = ((int)0x8C88),
        timestamp = ((int)0x8E28),
    };
}
namespace renderbuffer_target
{
    enum value
    {
        renderbuffer = ((int)0x8D41),
    };
}
namespace sampler_parameter
{
    enum value
    {
        texture_border_color = ((int)0x1004),
        texture_mag_filter = ((int)0x2800),
        texture_min_filter = ((int)0x2801),
        texture_wrap_s = ((int)0x2802),
        texture_wrap_t = ((int)0x2803),
        texture_wrap_r = ((int)0x8072),
        texture_min_lod = ((int)0x813a),
        texture_max_lod = ((int)0x813b),
        texture_max_anisotropy_ext = ((int)0x84fe),
        texture_lod_bias = ((int)0x8501),
        texture_compare_mode = ((int)0x884c),
        texture_compare_func = ((int)0x884d),
    };
}
namespace shader_parameter
{
    enum value
    {
        shader_type = ((int)0x8B4F),
        delete_status = ((int)0x8B80),
        compile_status = ((int)0x8B81),
        info_log_length = ((int)0x8B84),
        shader_source_length = ((int)0x8B88),
    };
}
namespace shader_type
{
    enum value
    {
        fragment_shader = ((int)0x8B30),
        vertex_shader = ((int)0x8B31),
        geometry_shader = ((int)0x8DD9),
        tess_evaluation_shader = ((int)0x8E87),
        tess_control_shader = ((int)0x8E88),
    };
}
namespace stencil_face
{
    enum value
    {
        front = ((int)0x0404),
        back = ((int)0x0405),
        front_and_back = ((int)0x0408),
    };
}
namespace stencil_function_enum
{
    enum value
    {
        never = ((int)0x0200),
        less = ((int)0x0201),
        equal = ((int)0x0202),
        lequal = ((int)0x0203),
        greater = ((int)0x0204),
        not_equal = ((int)0x0205),
        gequal = ((int)0x0206),
        always = ((int)0x0207),
    };
}
namespace stencil_op_enum
{
    enum value
    {
        zero = ((int)0),
        invert = ((int)0x150A),
        keep = ((int)0x1E00),
        replace = ((int)0x1E01),
        incr = ((int)0x1E02),
        decr = ((int)0x1E03),
        incr_wrap = ((int)0x8507),
        decr_wrap = ((int)0x8508),
    };
}
namespace texture_compare_mode
{
    enum value
    {
        none = ((int)0),
        compare_ref_to_texture = ((int)0x884E),
        compare_r_to_texture = ((int)0x884E),
    };
}
namespace texture_mag_filter
{
    enum value
    {
        nearest = ((int)0x2600),
        linear = ((int)0x2601),
    };
}
namespace texture_min_filter
{
    enum value
    {
        nearest = ((int)0x2600),
        linear = ((int)0x2601),
        nearest_mipmap_nearest = ((int)0x2700),
        linear_mipmap_nearest = ((int)0x2701),
        nearest_mipmap_linear = ((int)0x2702),
        linear_mipmap_linear = ((int)0x2703),
    };
}
namespace texture_parameter_name
{
    enum value
    {
        texture_border_color = ((int)0x1004),
        texture_mag_filter = ((int)0x2800),
        texture_min_filter = ((int)0x2801),
        texture_wrap_s = ((int)0x2802),
        texture_wrap_t = ((int)0x2803),
        texture_priority = ((int)0x8066),
        texture_depth = ((int)0x8071),
        texture_wrap_r = ((int)0x8072),
        texture_compare_fail_value = ((int)0x80bf),
        //clamp_to_border = ((int)0x812d),
        //clamp_to_edge = ((int)0x812f),
        texture_min_lod = ((int)0x813a),
        texture_max_lod = ((int)0x813b),
        texture_base_level = ((int)0x813c),
        texture_max_level = ((int)0x813d),
        generate_mipmap = ((int)0x8191),
        texture_lod_bias = ((int)0x8501),
        depth_texture_mode = ((int)0x884b),
        texture_compare_mode = ((int)0x884c),
        texture_compare_func = ((int)0x884d),
        texture_swizzle_r = ((int)0x8e42),
        texture_swizzle_g = ((int)0x8e43),
        texture_swizzle_b = ((int)0x8e44),
        texture_swizzle_a = ((int)0x8e45),
        texture_swizzle_rgba = ((int)0x8e46),
    };
}
namespace texture_level_parameter_name
{
    enum value
    {
        texture_width = ((int)0x1000),
        texture_height = ((int)0x1001),
        texture_depth = ((int)0x8071),
        texture_internal_format = ((int)0x1003),
        texture_border = ((int)0x1005),
        texture_red_size = ((int)0x805C),
        texture_green_size = ((int)0x805D),
        texture_blue_size = ((int)0x805E),
        texture_alpha_size = ((int)0x805F),
        texture_depth_size = ((int)0x884A),
        texture_compressed = ((int)0x86A1),
        texture_compressed_image_size = ((int)0x86A0)
    };
}
namespace texture_unit
{
    enum value
    {
        texture0 = ((int)0x84C0),
    };
}
namespace texture_target
{
    enum value
    {
        invalid = 0,
        texture_1d = ((int)0x0DE0),
        texture_2d = ((int)0x0DE1),
        //proxy_texture_1D = ((int)0x8063),
        //proxy_texture_2D = ((int)0x8064),
        texture_3d = ((int)0x806F),
        //proxy_texture_3D = ((int)0x8070),
        texture_rectangle = ((int)0x84F5),
        //proxy_texture_rectangle = ((int)0x84F7),
        texture_cubemap = ((int)0x8513),
        texture_binding_cubemap = ((int)0x8514),
        texture_cubemap_positive_x = ((int)0x8515),
        texture_cubemap_negative_x = ((int)0x8516),
        texture_cubemap_positive_y = ((int)0x8517),
        texture_cubemap_negative_y = ((int)0x8518),
        texture_cubemap_positive_z = ((int)0x8519),
        texture_cubemap_negative_z = ((int)0x851A),
        //proxy_texture_cubemap = ((int)0x851B),
        texture_1d_array = ((int)0x8C18),
        //proxy_texture_1d_array = ((int)0x8C19),
        texture_2d_array = ((int)0x8C1A),
        //proxy_texture_2d_array = ((int)0x8C1B),
        texture_buffer = ((int)0x8C2A),
        texture_cubemap_array = ((int)0x9009),
        //proxy_texture_cubemap_array = ((int)0x900B),
        texture_2d_multisample = ((int)0x9100),
        //proxy_texture_2d_multisample = ((int)0x9101),
        texture_2d_multisample_array = ((int)0x9102),
        //proxy_texture_2d_multisample_array = ((int)0x9103),
    };
}
namespace texture_wrap_mode
{
    enum value
    {
        clamp = ((int)0x2900),
        repeat = ((int)0x2901),
        clamp_to_border = ((int)0x812D),
        clamp_to_edge = ((int)0x812F),
        mirrored_repeat = ((int)0x8370),
    };
}
namespace transform_feedback_mode
{
    enum value
    {
        interleaved_attribs = ((int)0x8C8C),
        separate_attribs = ((int)0x8C8D),
    };
}
namespace transform_feedback_target
{
    enum value
    {
        transform_feedback = ((int)0x8E22),
    };
}
namespace vertex_attrib_pointer_type
{
    enum value
    {
        byte = ((int)0x1400),
        unsigned_byte = ((int)0x1401),
        short_ = ((int)0x1402),
        unsigned_short = ((int)0x1403),
        int_ = ((int)0x1404),
        unsigned_int = ((int)0x1405),
        float_ = ((int)0x1406),
        double_ = ((int)0x140A),
        half_float = ((int)0x140B),
        fixed = ((int)0x140C),
        unsigned_int_2101010_rev = ((int)0x8368),
        int_2101010_rev = ((int)0x8D9F),
    };
}

namespace fragment_output_type
{
    enum value
    {
        int_ = ((int)0x1404),
        int_vec2 = ((int)0x8B53),
        int_vec3 = ((int)0x8B54),
        int_vec4 = ((int)0x8B55),
        unsigned_int = ((int)0x1405),
        unsigned_int_vec2 = ((int)0x8DC6),
        unsigned_int_vec3 = ((int)0x8DC7),
        unsigned_int_vec4 = ((int)0x8DC8),
        float_ = ((int)0x1406),
        float_vec2 = ((int)0x8B50),
        float_vec3 = ((int)0x8B51),
        float_vec4 = ((int)0x8B52),
        double_ = ((int)0x140A),
        double_vec2 = ((int)0x8FFC),
        double_vec3 = ((int)0x8FFD),
        double_vec4 = ((int)0x8FFE),
    };
}

}

#endif
