/**
 * @defgroup Materials Materials
 * @ingroup Modules
 *
 * Define some useful materials.
 *
 * This module allows you to define and apply some useful materials on your different objects.
 *
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
 * Set a sort of plastic material, whose color is define by(@p r, @p g, @p b).
 *
 * @note This material act as a simple texture.
 * @note There is no one reflection on this material.
 *
 */
plastic(r:real, g:real, b:real);

/**
 * Defines a fresnel material.
 *
 * Create a material subject to fresnel reflection/refraction (for instance, @ref gold, @ref glass, etc.).
 *
 * @param r red component of the material color.
 * @param g green component of the material color.
 * @param b blue component of the material color.
 * @param a alpha-channel, transparency of the color. 0 define a no color material and 1 an fully color one.
 * @param n indice of refraction of the material.
 * @param roughness roughness of the material. 0 define a transparent material and 1 an opaque one.
 *
 * @attention A @ref cube_map has to be define previously in order to generate reflection/refraction effects.
 * @attention This material has to be apply just before the choosen object in order to be subject correctly to all its transformations.
 *
 * An example of use of this effect is described below
@code
// Define cube map to generate reflection
cube_map
   cube_map_face 0, "right.png"
   cube_map_face 1, "face.png"
   cube_map_face 2, "top.png"
   cube_map_face 3, "bottom.png"
   cube_map_face 4, "front.png"
   cube_map_face 5, "back.png"
rotatex time * 20
rotatey 90
// Apply effect just before drawing object
fresnel_material 1.0, 1.0, 1.0, 0.1, 1.12, 0.01
sphere 0, 0, 0, 500, 500, 500, 50, 50
@endcode
 *
 * @note This material act as a simple texture.
 *
 * @see Some indices of refraction can be found at http://www.pixelandpoly.com/ior.html
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
fresnel_material 0.65, 0.607, 0.031, 0.7, 0.47, 1.0
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
fresnel_material 0.807, 0.807, 0.807, 0.4, 0.8, 1.0
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
fresnel_material 0.34, 0.24, 0.1, 0.93, 1.18, 1.0
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
fresnel_material 0.43, 0.376, 0.364, 0.6, 2.95, 1.0
@endcode
 *
 */
iron();

/**
 * Defines a sort of chromium material.
 *
 * Apply a sort of chromium material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
fresnel_material 0.8, 0.8, 0.8, 0.1, 2.97, 1.0
@endcode
 *
 */
chromium();

/**
 * Defines a sort of emerald material.
 *
 * Apply a sort of emerald material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
fresnel_material 0.0, 0.6, 0.1, 0.65, 1.56, 0.03
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
fresnel_material 0.6, 0.1, 0.1, 0.7, 1.757, 0.03
@endcode
 *
 */
ruby();

/**
 * Defines a sort of diamond material.
 *
 * Apply a sort of diamond material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
fresnel_material 1.0, 1.0, 1.0, 0.1, 2.458, 0.03
@endcode
 *
 */
diamond();

/**
 * Defines a sort of glass material.
 *
 * Apply a sort of glass material to an object.
 *
 * @attention This material is subject to the same remarks than @ref fresnel_material in order to be apply correctly on an object.
 *
 * @note It can be also define with
@code
fresnel_material 1.0, 1.0, 1.0, 0.1, 1.12, 0.01
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
fresnel_material 0.35, 0.5, 0.5, 0.5, 1.325, 0.01
@endcode
 *
 */
water();


/**
* @}
*/
