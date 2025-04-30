#include "../pch.h"
#include "ScriptComponent.h"

// GLM bindings from https://github.com/dwjclark11/Scion2D/blob/master/SCION_CORE/src/Scripting/GlmLuaBindings.cpp, distributed under the MIT license.
// glm::vec2
void CreateVec2Bind(sol::state& lua)
{
	// multiplier overloads
	auto vec2_multiply_overloads = sol::overload([](const glm::vec2& v1, const glm::vec2& v2) { return v1 * v2; },
		[](const glm::vec2& v1, float value) { return v1 * value; },
		[](float value, const glm::vec2& v1) { return v1 * value; });

	// divider overloads
	auto vec2_divide_overloads = sol::overload([](const glm::vec2& v1, const glm::vec2& v2) { return v1 / v2; },
		[](const glm::vec2& v1, float value) { return v1 / value; },
		[](float value, const glm::vec2& v1) { return v1 / value; });

	// addition overloads
	auto vec2_addition_overloads = sol::overload([](const glm::vec2& v1, const glm::vec2& v2) { return v1 + v2; },
		[](const glm::vec2& v1, float value) { return v1 + value; },
		[](float value, const glm::vec2& v1) { return v1 + value; });

	// subtraction overloads
	auto vec2_subtraction_overloads = sol::overload([](const glm::vec2& v1, const glm::vec2& v2) { return v1 - v2; },
		[](const glm::vec2& v1, float value) { return v1 - value; },
		[](float value, const glm::vec2& v1) { return v1 - value; });

	// create vec2 usertype
	lua.new_usertype<glm::vec2>(
		"vec2",
		sol::call_constructor,
		sol::constructors<glm::vec2(float), glm::vec2(float, float)>(),
		"x",
		&glm::vec2::x,
		"y",
		&glm::vec2::y,
		sol::meta_function::multiplication,
		vec2_multiply_overloads,
		sol::meta_function::division,
		vec2_divide_overloads,
		sol::meta_function::addition,
		vec2_addition_overloads,
		sol::meta_function::subtraction,
		vec2_subtraction_overloads,
		"length",
		[](const glm::vec2& v) { return glm::length(v); },
		"lengthSq",
		[](const glm::vec2& v) { return glm::length2(v); },
		"normalize",
		[](const glm::vec2& v1) { return glm::normalize(v1); },
		"normalize2",
		[](const glm::vec2& v1, const glm::vec2& v2) { return glm::normalize(v2 - v1); },
		"nearly_zero_x",
		[](const glm::vec2& v) { return glm::epsilonEqual(v.x, 0.f, 0.001f); },
		"nearly_zero_y",
		[](const glm::vec2& v) { return glm::epsilonEqual(v.y, 0.f, 0.001f); });
}

// glm::vec3
void CreateVec3Bind(sol::state& lua)
{
	// multiplier overloads
	auto vec3_multiply_overloads = sol::overload([](const glm::vec3& v1, const glm::vec3& v2) { return v1 * v2; },
		[](const glm::vec3& v1, float value) { return v1 * value; },
		[](float value, const glm::vec3& v1) { return v1 * value; });

	// divider overloads
	auto vec3_divide_overloads = sol::overload([](const glm::vec3& v1, const glm::vec3& v2) { return v1 / v2; },
		[](const glm::vec3& v1, float value) { return v1 / value; },
		[](float value, const glm::vec3& v1) { return v1 / value; });

	// addition overloads
	auto vec3_addition_overloads = sol::overload([](const glm::vec3& v1, const glm::vec3& v2) { return v1 + v2; },
		[](const glm::vec3& v1, float value) { return v1 + value; },
		[](float value, const glm::vec3& v1) { return v1 + value; });

	// subtraction overloads
	auto vec3_subtraction_overloads = sol::overload([](const glm::vec3& v1, const glm::vec3& v2) { return v1 - v2; },
		[](const glm::vec3& v1, float value) { return v1 - value; },
		[](float value, const glm::vec3& v1) { return v1 - value; });

    auto vec3_print_overloads = sol::overload([](const glm::vec3& v) { return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z); });

	// create vec3 usertype
	lua.new_usertype<glm::vec3>(
		"vec3",
		sol::call_constructor,
		sol::constructors<
			glm::vec3(float),
			glm::vec3(float, float, float),
			glm::vec3(),
			glm::vec3(double),
			glm::vec3(double, double, double),
			glm::vec3(glm::dvec3)
		>(),
		"x",
		&glm::vec3::x,
		"y",
		&glm::vec3::y,
		"z",
		&glm::vec3::z,
		sol::meta_function::multiplication,
		vec3_multiply_overloads,
		sol::meta_function::division,
		vec3_divide_overloads,
		sol::meta_function::addition,
		vec3_addition_overloads,
		sol::meta_function::subtraction,
		vec3_subtraction_overloads,
        sol::meta_function::to_string,
		vec3_print_overloads,
		// Negation operator binding
		sol::meta_function::unary_minus, [](const glm::vec3& v) {
			return -v;
		},
		"length",
		[](const glm::vec3& v) { return glm::length(v); },
		"lengthSq",
		[](const glm::vec3& v) { return glm::length2(v); },
		"normalize",
		[](const glm::vec3& v) { return glm::length(v) > 0.0f ? glm::normalize(v) : glm::vec3(0); },
		"normalize2",
		[](const glm::vec3& v1, const glm::vec3& v2) { return glm::normalize(v2 - v1); },
		"nearly_zero_x",
		[](const glm::vec3& v) { return glm::epsilonEqual(v.x, 0.f, 0.001f); },
		"nearly_zero_y",
		[](const glm::vec3& v) { return glm::epsilonEqual(v.y, 0.f, 0.001f); },
		"nearly_zero_z",
		[](const glm::vec3& v) { return glm::epsilonEqual(v.z, 0.f, 0.001f); });
}

// glm::vec4
void CreateVec4Bind(sol::state& lua)
{
	// multiplier overloads
	auto vec4_multiply_overloads = sol::overload([](const glm::vec4& v1, const glm::vec4& v2) { return v1 * v2; },
		[](const glm::vec4& v1, float value) { return v1 * value; },
		[](float value, const glm::vec4& v1) { return v1 * value; });

	// divider overloads
	auto vec4_divide_overloads = sol::overload([](const glm::vec4& v1, const glm::vec4& v2) { return v1 / v2; },
		[](const glm::vec4& v1, float value) { return v1 / value; },
		[](float value, const glm::vec4& v1) { return v1 / value; });

	// addition overloads
	auto vec4_addition_overloads = sol::overload([](const glm::vec4& v1, const glm::vec4& v2) { return v1 + v2; },
		[](const glm::vec4& v1, float value) { return v1 + value; },
		[](float value, const glm::vec4& v1) { return v1 + value; });

	// subtraction overloads
	auto vec4_subtraction_overloads = sol::overload([](const glm::vec4& v1, const glm::vec4& v2) { return v1 - v2; },
		[](const glm::vec4& v1, float value) { return v1 - value; },
		[](float value, const glm::vec4& v1) { return v1 - value; });

	// create vec4 usertype
	lua.new_usertype<glm::vec4>(
		"vec4",
		sol::call_constructor,
		sol::constructors<glm::vec4(float), glm::vec4(float, float, float, float)>(),
		"x",
		&glm::vec4::x,
		"y",
		&glm::vec4::y,
		"z",
		&glm::vec4::z,
		"w",
		&glm::vec4::w,
		sol::meta_function::multiplication,
		vec4_multiply_overloads,
		sol::meta_function::division,
		vec4_divide_overloads,
		sol::meta_function::addition,
		vec4_addition_overloads,
		sol::meta_function::subtraction,
		vec4_subtraction_overloads,
		"length",
		[](const glm::vec4& v) { return glm::length(v); },
		"lengthSq",
		[](const glm::vec4& v) { return glm::length2(v); },
		"normalize",
		[](const glm::vec4& v1) { return glm::normalize(v1); },
		"normalize2",
		[](const glm::vec4& v1, const glm::vec4& v2) { return glm::normalize(v2 - v1); },
		"nearly_zero_x",
		[](const glm::vec4& v) { return glm::epsilonEqual(v.x, 0.f, 0.001f); },
		"nearly_zero_y",
		[](const glm::vec4& v) { return glm::epsilonEqual(v.y, 0.f, 0.001f); },
		"nearly_zero_z",
		[](const glm::vec4& v) { return glm::epsilonEqual(v.z, 0.f, 0.001f); },
		"nearly_zero_w",
		[](const glm::vec4& v) { return glm::epsilonEqual(v.w, 0.f, 0.001f); });
}

//=======================
// Double Vectors
//=======================

void CreateDVec2Bind(sol::state& lua) {
	auto mult_overloads = sol::overload(
		[](const glm::dvec2& v1, const glm::dvec2& v2) { return v1 * v2; },
		[](const glm::dvec2& v1, double value) { return v1 * value; },
		[](double value, const glm::dvec2& v1) { return v1 * value; }
	);

	auto div_overloads = sol::overload(
		[](const glm::dvec2& v1, const glm::dvec2& v2) { return v1 / v2; },
		[](const glm::dvec2& v1, double value) { return v1 / value; },
		[](double value, const glm::dvec2& v1) { return v1 / value; }
	);

	auto add_overloads = sol::overload(
		[](const glm::dvec2& v1, const glm::dvec2& v2) { return v1 + v2; },
		[](const glm::dvec2& v1, double value) { return v1 + value; },
		[](double value, const glm::dvec2& v1) { return v1 + value; }
	);

	auto sub_overloads = sol::overload(
		[](const glm::dvec2& v1, const glm::dvec2& v2) { return v1 - v2; },
		[](const glm::dvec2& v1, double value) { return v1 - value; },
		[](double value, const glm::dvec2& v1) { return v1 - value; }
	);

	lua.new_usertype<glm::dvec2>("dvec2",
		sol::call_constructor,
		sol::constructors<
		glm::dvec2(),
		glm::dvec2(double),
		glm::dvec2(double, double)
		>(),
		"x", &glm::dvec2::x,
		"y", &glm::dvec2::y,
		sol::meta_function::multiplication, mult_overloads,
		sol::meta_function::division, div_overloads,
		sol::meta_function::addition, add_overloads,
		sol::meta_function::subtraction, sub_overloads,
		"length", [](const glm::dvec2& v) { return glm::length(v); },
		"lengthSq", [](const glm::dvec2& v) { return glm::length2(v); },
		"normalize", [](const glm::dvec2& v) { return glm::normalize(v); },
		"normalize2", [](const glm::dvec2& v1, const glm::dvec2& v2) {
			return glm::normalize(v2 - v1);
		},
		"nearly_zero_x", [](const glm::dvec2& v) {
			return glm::epsilonEqual(v.x, 0.0, 0.001);
		},
		"nearly_zero_y", [](const glm::dvec2& v) {
			return glm::epsilonEqual(v.y, 0.0, 0.001);
		}
	);
}

void CreateDVec3Bind(sol::state& lua) {
	auto mult_overloads = sol::overload(
		[](const glm::dvec3& v1, const glm::dvec3& v2) { return v1 * v2; },
		[](const glm::dvec3& v1, double value) { return v1 * value; },
		[](double value, const glm::dvec3& v1) { return v1 * value; }
	);

	auto div_overloads = sol::overload(
		[](const glm::dvec3& v1, const glm::dvec3& v2) { return v1 / v2; },
		[](const glm::dvec3& v1, double value) { return v1 / value; },
		[](double value, const glm::dvec3& v1) { return v1 / value; }
	);

	auto add_overloads = sol::overload(
		[](const glm::dvec3& v1, const glm::dvec3& v2) { return v1 + v2; },
		[](const glm::dvec3& v1, double value) { return v1 + value; },
		[](double value, const glm::dvec3& v1) { return v1 + value; }
	);

	auto sub_overloads = sol::overload(
		[](const glm::dvec3& v1, const glm::dvec3& v2) { return v1 - v2; },
		[](const glm::dvec3& v1, double value) { return v1 - value; },
		[](double value, const glm::dvec3& v1) { return v1 - value; }
	);

	lua.new_usertype<glm::dvec3>("dvec3",
		sol::call_constructor,
		sol::constructors<
		glm::dvec3(),
		glm::dvec3(double),
		glm::dvec3(double, double, double),
		glm::dvec3(float),
        glm::dvec3(float, float, float),
		glm::dvec3(glm::vec3)
		>(),
		"x", &glm::dvec3::x,
		"y", &glm::dvec3::y,
		"z", &glm::dvec3::z,
		sol::meta_function::multiplication, mult_overloads,
		sol::meta_function::division, div_overloads,
		sol::meta_function::addition, add_overloads,
		sol::meta_function::subtraction, sub_overloads,
		"length", [](const glm::dvec3& v) { return glm::length(v); },
		"lengthSq", [](const glm::dvec3& v) { return glm::length2(v); },
		"normalize", [](const glm::dvec3& v) { return glm::normalize(v); },
		"normalize2", [](const glm::dvec3& v1, const glm::dvec3& v2) {
			return glm::normalize(v2 - v1);
		},
		"nearly_zero_x", [](const glm::dvec3& v) {
			return glm::epsilonEqual(v.x, 0.0, 0.001);
		},
		"nearly_zero_y", [](const glm::dvec3& v) {
			return glm::epsilonEqual(v.y, 0.0, 0.001);
		},
		"nearly_zero_z", [](const glm::dvec3& v) {
			return glm::epsilonEqual(v.z, 0.0, 0.001);
		}
	);
}

void CreateDVec4Bind(sol::state& lua) {
	auto mult_overloads = sol::overload(
		[](const glm::dvec4& v1, const glm::dvec4& v2) { return v1 * v2; },
		[](const glm::dvec4& v1, double value) { return v1 * value; },
		[](double value, const glm::dvec4& v1) { return v1 * value; }
	);

	auto div_overloads = sol::overload(
		[](const glm::dvec4& v1, const glm::dvec4& v2) { return v1 / v2; },
		[](const glm::dvec4& v1, double value) { return v1 / value; },
		[](double value, const glm::dvec4& v1) { return v1 / value; }
	);

	auto add_overloads = sol::overload(
		[](const glm::dvec4& v1, const glm::dvec4& v2) { return v1 + v2; },
		[](const glm::dvec4& v1, double value) { return v1 + value; },
		[](double value, const glm::dvec4& v1) { return v1 + value; }
	);

	auto sub_overloads = sol::overload(
		[](const glm::dvec4& v1, const glm::dvec4& v2) { return v1 - v2; },
		[](const glm::dvec4& v1, double value) { return v1 - value; },
		[](double value, const glm::dvec4& v1) { return v1 - value; }
	);

	lua.new_usertype<glm::dvec4>("dvec4",
		sol::call_constructor,
		sol::constructors<
		glm::dvec4(),
		glm::dvec4(double),
		glm::dvec4(double, double, double, double)
		>(),
		"x", &glm::dvec4::x,
		"y", &glm::dvec4::y,
		"z", &glm::dvec4::z,
		"w", &glm::dvec4::w,
		sol::meta_function::multiplication, mult_overloads,
		sol::meta_function::division, div_overloads,
		sol::meta_function::addition, add_overloads,
		sol::meta_function::subtraction, sub_overloads,
		"length", [](const glm::dvec4& v) { return glm::length(v); },
		"lengthSq", [](const glm::dvec4& v) { return glm::length2(v); },
		"normalize", [](const glm::dvec4& v) { return glm::normalize(v); },
		"normalize2", [](const glm::dvec4& v1, const glm::dvec4& v2) {
			return glm::normalize(v2 - v1);
		},
		"nearly_zero_x", [](const glm::dvec4& v) {
			return glm::epsilonEqual(v.x, 0.0, 0.001);
		},
		"nearly_zero_y", [](const glm::dvec4& v) {
			return glm::epsilonEqual(v.y, 0.0, 0.001);
		},
		"nearly_zero_z", [](const glm::dvec4& v) {
			return glm::epsilonEqual(v.z, 0.0, 0.001);
		},
		"nearly_zero_w", [](const glm::dvec4& v) {
			return glm::epsilonEqual(v.w, 0.0, 0.001);
		}
	);
}

// Similar implementations for dvec3 and dvec4 following the same pattern
// with appropriate component counts and constructor parameters

//=======================
// Matrices
//=======================

void CreateMat2Bind(sol::state& lua) {
	auto mat_mult_overloads = sol::overload(
		[](const glm::mat2& m1, const glm::mat2& m2) { return m1 * m2; },
		[](const glm::mat2& m, const glm::vec2& v) { return m * v; },
		[](const glm::mat2& m, float s) { return m * s; },
		[](float s, const glm::mat2& m) { return m * s; }
	);

	lua.new_usertype<glm::mat2>("mat2",
		sol::call_constructor,
		sol::constructors<
		glm::mat2(),
		glm::mat2(float),
		glm::mat2(float, float, float, float)
		>(),
		sol::meta_function::multiplication, mat_mult_overloads,
		"transpose", [](const glm::mat2& m) { return glm::transpose(m); },
		"determinant", [](const glm::mat2& m) { return glm::determinant(m); },
		"inverse", [](const glm::mat2& m) { return glm::inverse(m); },
		"identity", []() { return glm::mat2(1.0f); }
	);
}

void CreateMat3Bind(sol::state& lua) {
	auto mat_mult_overloads = sol::overload(
		[](const glm::mat3& m1, const glm::mat3& m2) { return m1 * m2; },
		[](const glm::mat3& m, const glm::vec3& v) { return m * v; },
		[](const glm::mat3& m, float s) { return m * s; },
		[](float s, const glm::mat3& m) { return m * s; }
	);

	lua.new_usertype<glm::mat3>("mat3",
		sol::call_constructor,
		sol::constructors<
		glm::mat3(),
		glm::mat3(float),
		glm::mat3(
			float, float, float,
			float, float, float,
			float, float, float)
		>(),
		sol::meta_function::multiplication, mat_mult_overloads,
		"transpose", [](const glm::mat3& m) { return glm::transpose(m); },
		"determinant", [](const glm::mat3& m) { return glm::determinant(m); },
		"inverse", [](const glm::mat3& m) { return glm::inverse(m); },
		"identity", []() { return glm::mat3(1.0f); }
	);
}

auto mat4_print_overloads = sol::overload([](const glm::mat4& v) {
    return std::to_string(v[0][0]) + ", " + std::to_string(v[0][1]) + ", " + std::to_string(v[0][2]) + ", " + std::to_string(v[0][3]) + "\n" +
        std::to_string(v[1][0]) + ", " + std::to_string(v[1][1]) + ", " + std::to_string(v[1][2]) + ", " + std::to_string(v[1][3]) + "\n" +
        std::to_string(v[2][0]) + ", " + std::to_string(v[2][1]) + ", " + std::to_string(v[2][2]) + ", " + std::to_string(v[2][3]) + "\n" +
        std::to_string(v[3][0]) + ", " + std::to_string(v[3][1]) + ", " + std::to_string(v[3][2]) + ", " + std::to_string(v[3][3]);
});
void CreateMat4Bind(sol::state& lua) {
	auto mat_mult_overloads = sol::overload(
		[](const glm::mat4& m1, const glm::mat4& m2) { return m1 * m2; },
		[](const glm::mat4& m, const glm::vec4& v) { return m * v; },
		[](const glm::mat4& m, float s) { return m * s; },
		[](float s, const glm::mat4& m) { return m * s; }
	);

	lua.new_usertype<glm::mat4>("mat4",
		sol::call_constructor,
		sol::constructors<
		glm::mat4(),
		glm::mat4(float),
		glm::mat4(
			float, float, float, float,
			float, float, float, float,
			float, float, float, float,
			float, float, float, float)
		>(),
		sol::meta_function::multiplication, mat_mult_overloads,
		sol::meta_function::to_string, mat4_print_overloads,
		"transpose", [](const glm::mat4& m) { return glm::transpose(m); },
		"determinant", [](const glm::mat4& m) { return glm::determinant(m); },
		"inverse", [](const glm::mat4& m) { return glm::inverse(m); },
		"identity", []() { return glm::mat4(1.0f); },
		"translate", [](const glm::mat4& m, const glm::vec3& v) { return glm::translate(m, v); },
		"rotate", [](const glm::mat4& m, float angle, const glm::vec3& axis) { return glm::rotate(m, angle, axis); },
		"scale", [](const glm::mat4& m, const glm::vec3& s) { return glm::scale(m, s); }
	);
}

//=======================
// Main Binding Function
//=======================

void BindGLMTypes(sol::state& lua) {
	
}

void CreateQuaternionLuaBind(sol::state& lua)
{
	/* This might not be used */
	lua.new_usertype<glm::quat>(
		"quat",
		sol::call_constructor,
		sol::constructors<glm::quat(float, float, float, float)>(),
		"x",
		&glm::quat::x,
		"y",
		&glm::quat::y,
		"z",
		&glm::quat::z,
		"w",
		&glm::quat::w,
		"normalize",
		[](const glm::quat& q) { return glm::normalize(q); },
		"conjugate",
		[](const glm::quat& q) { return glm::conjugate(q); },
		"cross",
		[](const glm::quat& q, const glm::quat& q2) { return glm::cross(q, q2); },
		"dot",
		[](const glm::quat& q, const glm::quat& q2) { return glm::dot(q, q2); });
}

/*
 * Some helper math functions
 */
void MathFreeFunctions(sol::state& lua)
{
	lua.set_function("SE_distance",
		sol::overload([](glm::vec2& a, glm::vec2& b) { return glm::distance(a, b); },
			[](glm::vec3& a, glm::vec3& b) { return glm::distance(a, b); },
			[](glm::vec4& a, glm::vec4& b) { return glm::distance(a, b); }));

	lua.set_function("SE_lerp", [](float a, float b, float t) { return std::lerp(a, b, t); });
	lua.set_function(
		"SE_clamp",
		sol::overload([](float value, float min, float max) { return std::clamp(value, min, max); },
			[](double value, double min, double max) { return std::clamp(value, min, max); },
			[](int value, int min, int max) { return std::clamp(value, min, max); }));

	lua.set_function("SE_nearly_zero",
		sol::overload(
			[](const glm::vec2& v) {
				return glm::epsilonEqual(v.x, 0.f, 0.001f) && glm::epsilonEqual(v.y, 0.f, 0.001f);
			},
			[](const glm::vec3& v) {
				return glm::epsilonEqual(v.x, 0.f, 0.001f) && glm::epsilonEqual(v.y, 0.f, 0.001f) &&
					glm::epsilonEqual(v.z, 0.f, 0.001f);
			}));

	lua.set_function("SE_dot_product",
		sol::overload([](const glm::vec2& v1, const glm::vec2& v2) { return glm::dot(v1, v2); },
			[](const glm::vec3& v1, const glm::vec3& v2) { return glm::dot(v1, v2); },
			[](const glm::vec4& v1, const glm::vec4& v2) { return glm::dot(v1, v2); }));

	lua.set_function(
		"SE_cross_product",
		sol::overload([](const glm::vec2& v1, const glm::vec2& v2) { return v1.x * v2.y - v2.x * v1.y; },
			[](const glm::vec3& v1, const glm::vec3& v2) { return glm::cross(v1, v2); }));
}

void MathConstants(sol::state& lua)
{
	lua.set("SE_PI", 3.14159265359f);
	lua.set("SE_TWO_PI", 6.28318530717f);
	lua.set("SE_PI_SQUARED", 9.86960440108f);
	lua.set("SE_PI_OVER_2", 1.57079632679f);
	lua.set("SE_PI_OVER_4", 0.78539816339f);
	lua.set("SE_PHI", 1.6180339887498948482045868343656381f);
	lua.set("SE_EULERS", 2.71828182845904523536f);

	lua.set("SE_SQRT_2", 1.4142135623730950488016887242097f);
	lua.set("SE_SQRT_3", 1.7320508075688772935274463415059f);
	lua.set("SE_INV_SQRT_2", 0.70710678118654752440084436210485f);
	lua.set("SE_INV_SQRT_3", 0.57735026918962576450914878050196f);
}

void ScriptComponent::CreateGLMBindings()
{
	CreateVec2Bind(m_lua);
	CreateVec3Bind(m_lua);
	CreateVec4Bind(m_lua);
	CreateDVec2Bind(m_lua);
	CreateDVec3Bind(m_lua);
	CreateDVec4Bind(m_lua);
	CreateMat2Bind(m_lua);
	CreateMat3Bind(m_lua);
	CreateMat4Bind(m_lua);

	MathFreeFunctions(m_lua);
	MathConstants(m_lua);
}