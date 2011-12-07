/**
 * @defgroup Materials Materials
 * @ingroup Modules
 *
 * Define some useful materials.
 *
 * This module allows you to define and apply some useful materials on your different objects.
 * The following image shows a set of some defined materials.
 *
 * @image html materials.png "Example of materials"
 *
 * @note Materials defined in this module are adapted for a basic usage.
 * @note For more complex scene, it is recommended to define directly your own materials
 * by using combinaisons of available functions in this module and in Tao.
 *
 * @{
 */

/**
 * Defines a sort of plastic material.
 *
 * Set a sort of plastic material, whose color is defined by @ref graph.doxy.h::color(r:real,g:real,b:real,a:real).
 *
 * @note This material act as a simple texture.
 * @note There is no one reflection on this material.
 *
 */
plastic();

/**
 * Defines the first color of the marble material.
 *
 * Set color define by (@p r, @p g, @p b) as first color of the marble material.
 * It corresponds to the color of the regions with no veining.
 * The default value is (1.0, 1.0, 1.0).
 */
marble_first_color(r:real, g:real, b:real);

/**
 * Defines the first color of the marble material.
 *
 * Set color define by (@p r, @p g, @p b) as first color of the marble material.
 * It corresponds to the color of the veining.
 * The default value is (0.60, 0.50, 0.50).
 */
marble_second_color(r:real, g:real, b:real);

/**
 * Defines a sort of marble material.
 *
 * Set a sort of marble material, whose colors are defined by @ref marble_first_color and @ref marble_second_color.
 *
 * @param scale scaling parameter of the material.
 * @param seed distribution of the veining. In this case, there is limit of 200 seeds.
 *
 * @note This material is a procedural texture.
 */
marble(scale:real, seed:integer);

/**
 * Defines a default sort of marble material.
 *
 * Set a sort of marble material, whose colors are defined by @ref marble_first_color and @ref marble_second_color.
 *
 * @param scale scaling parameter of the material.
 *
 * @note This material is a procedural texture.
 * @note This function is a shortcut to the following code.
@code
marble scale, 128
@endcode
 */
marble(scale:real);

/**
 * Defines the first color of the granite material.
 *
 * Set color define by(@p r, @p g, @p b) as first color of the granite material.
 * It corresponds to the color of the regions with no veining.
 * The default value is (0.30, 0.15, 0.10).
 */
granite_first_color(r:real, g:real, b:real);

/**
 * Defines the first color of the granite material.
 *
 * Set color define by (@p r, @p g, @p b) as first color of the granite material.
 * It corresponds to the color of the veining.
 * The default value is (0.80, 0.73, 0.71).
 */
granite_second_color(r:real, g:real, b:real);

/**
 * Defines the first color of the marble material.
 *
 * Set color define by (@p r, @p g, @p b) as first color of the marble material.
 * It corresponds to the color of the marks.
 * The default value is (0.20, 0.20, 0.20).
 */
granite_third_color(r:real, g:real, b:real);

/**
 * Defines a sort of granite material.
 *
 * Set a sort of granite material, whose colors are defined by @ref granite_first_color, @ref granite_second_color
 * and @ref granite_third_color.
 *
 * @param scale scaling parameter of the material.
 * @param seed distribution of the veining. In this case, there is limit of 200 seeds.
 *
 * @note This material is a procedural texture.
 */
granite(scale:real, seed:integer);

/**
 * Defines a default sort of granite material.
 *
 * Set a sort of granite material, whose colors are defined by @ref granite_first_color, @ref granite_second_color
 * and @ref granite_third_color.
 *
 * @param scale Scaling parameter of the material.
 *
 * @note This material is a procedural texture.
 * @note This function is a shortcut to the following code.
@code
granite scale, 128
@endcode
 */
granite(scale:real);

/**
 * Defines the first color of the wood material.
 *
 * Set color define by (@p r, @p g, @p b) as first color of the wood material.
 * It corresponds to the color of the regions with no grain.
 * The default value is (0.92, 0.51, 0.13).
 */
wood_first_color(r:real, g:real, b:real);

/**
 * Defines the second color of the wood material.
 *
 * Set color define by (@p r, @p g, @p b) as first color of the wood material.
 * It corresponds to the color of the grain.
 * The default value is (0.44, 0.21, 0.0).
 */
wood_second_color(r:real, g:real, b:real);

/**
 * Defines a sort of wood material.
 *
 * Set a sort of wood material, whose colors are defined by @ref wood_first_color and @ref wood_second_color.
 *
 * @param scale Scaling parameter of the material.
 * @param ring Size of the rings
 * @param noise Influence of the noise on the material.
 * @param seed distribution of the grain. In this case, there is limit of 200 seeds.
 *
 * @note This material is a procedural texture.
 */
wood(scale:real, ring:real, noise:real, seed:integer);

/**
 * Defines a default sort of wood material.
 *
 * Set a sort of wood material, whose colors are defined by @ref wood_first_color and @ref wood_second_color.
 *
 * @param scale Scaling parameter of the material.
 *
 * @note This material is a procedural texture.
 * @note This function is a shortcut to the following code.
@code
wood scale, 20.0, 0.87, 128
@endcode
 */
wood(scale:real);

/**
 * Defines a fresnel material.
 *
 * Create a material subject to fresnel reflection/refraction (for instance, @ref gold, @ref glass, etc.).
 *
 * @param r bending color ratio. 0 define a no color material and 1 an fully color one.
 * @param n indice of refraction of the material.
 * @param roughness roughness of the material. 0 define a fully transparent material and 10 a fully opaque one.
 *
 * @attention A @ref cube_map has to be define previously in order to generate reflection/refraction effects.
 * @attention This material has to be apply just before the choosen object in order to be subject correctly to all its transformations.
 *
 * An example of use of this effect is described below
@code
// Define cube map to generate reflection
cube_map 512,
   cube_map_face 0, "right.png"
   cube_map_face 1, "face.png"
   cube_map_face 2, "top.png"
   cube_map_face 3, "bottom.png"
   cube_map_face 4, "front.png"
   cube_map_face 5, "back.png"
rotatex time * 20
rotatey 90
material_ambient 0.0, 0.0, 0.0, 0.0
material_diffuse 0.588235, 0.670588, 0.729412, 1.0
material_specular 0.9, 0.9, 0.9, 1.0
material_shininess 96.0
// Apply effect just before drawing object
fresnel_material 0.2, 1.12, 0.01
sphere 0, 0, 0, 500, 500, 500, 50, 50
@endcode
 *
 * @note This material act as any others materials.
 *
 * @see Some indices of refraction can be found at http://www.pixelandpoly.com/ior.html
 * @see Some material colors can be found at http://www.opengl.org/resources/code/samples/sig99/advanced99/notes/node153.html
 */
fresnel_material(r:real, g:real, b:real, a:real, n:real, roughness:real);

/**
 * Defines a sort of gold material.
 *
 * Apply a sort of gold material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.24725, 0.1995, 0.0745, 1.0
material_diffuse 0.75164, 0.60648, 0.22648, 1.0
material_specular 0.628281, 0.555802, 0.366065, 1.0
material_shininess 51.2
fresnel_material 0.7, 0.47, 10.0
@endcode
 *
 */
gold();

/**
 * Defines a sort of silver material.
 *
 * Apply a sort of silver material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.19225, 0.19225, 0.19225, 1.0
material_diffuse 0.50754, 0.50754, 0.50754, 1.0
material_specular 0.508273, 0.508273, 0.508273, 1.0
material_shininess 51.2
fresnel_material 0.7, 0.8, 10.0
@endcode
 *
 */
silver();

/**
 * Defines a sort of bronze material.
 *
 * Apply a sort of bronze material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.2125, 0.1275, 0.054, 1.0
material_diffuse 0.714, 0.4284, 0.18144, 1.0
material_specular 0.3935, 0.2719, 0.1667, 1.0
material_shininess 25.6
fresnel_material 0.93, 1.18, 10.0
@endcode
 *
 */
bronze();

/**
 * Defines a sort of iron material.
 *
 * Apply a sort of iron material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.19225, 0.19225, 0.19225, 1.0
material_diffuse 0.43, 0.376, 0.364, 1.0
material_specular 0.508273, 0.508273, 0.508273, 1.0
material_shininess 51.2
fresnel_material 0.6, 2.95, 1.0
@endcode
 *
 */
iron();

/**
 * Defines a sort of chrome material.
 *
 * Apply a sort of chrome material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.25, 0.25, 0.25, 1.0
material_diffuse 0.4, 0.4, 0.4, 1.0
material_specular 0.774597, 0.774597, 0.774597, 1.0
material_shininess 76.8
fresnel_material 0.35, 2.705, 10.0
@endcode
 *
 */
chrome();

/**
 * Defines a sort of emerald material.
 *
 * Apply a sort of emerald material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.0215, 0.1745, 0.0215, 0.55
material_diffuse 0.07568, 0.61424, 0.07568, 0.55
material_specular 0.633, 0.727811, 0.633, 0.55
material_shininess 76.8
fresnel_material 0.65, 1.56, 0.01
@endcode
 *
 */
emerald();

/**
 * Defines a sort of ruby material.
 *
 * Apply a sort of ruby material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.1745, 0.01175, 0.01175, 0.55
material_diffuse 0.61424, 0.04136, 0.04136, 0.55
material_specular 0.727811, 0.626959, 0.626959, 0.55
material_shininess 76.8
fresnel_material 0.7, 1.757, 0.01
@endcode
 *
 */
ruby();

/**
 * Defines a sort of jade material.
 *
 * Apply a sort of jade material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.135, 0.2225, 0.1575, 0.95
material_diffuse 0.54, 0.89, 0.63, 0.95
material_specular 0.316228, 0.316228, 0.316228, 0.95
material_shininess 12.8
fresnel_material 0.7, 1.64, 10.0
@endcode
 *
 */
jade();

/**
 * Defines a sort of glass material.
 *
 * Apply a sort of glass material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.0, 0.0, 0.0, 0.0
material_diffuse 0.588235, 0.670588, 0.729412, 1.0
material_specular 0.9, 0.9, 0.9, 1.0
material_shininess 96.0
fresnel_material 0.2, 1.12, 0.01
@endcode
 *
 */
glass();

/**
 * Defines a sort of water material.
 *
 * Apply a sort of water material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material to be apply correctly on an object.
 *
 * @note It can be also define with
@code
material_ambient 0.0, 0.0, 0.0, 0.0
material_diffuse  0.35, 0.5, 0.5, 1.0
material_specular 0.9, 0.9, 0.9, 1.0
material_shininess 76.8
fresnel_material 0.5, 1.325, 0.01
@endcode
 *
 */
water();


/**
* @}
*/
