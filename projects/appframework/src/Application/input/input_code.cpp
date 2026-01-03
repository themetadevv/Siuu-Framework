
#include "afpch.h"
#include "input_code.h"

std::string GetKeyCodeString(KeyCode key_code) {
	switch (key_code) {
	case KeyCode::Space:        return "Space"; break;
	case KeyCode::Apostrophe:   return "Apostrophe"; break;
	case KeyCode::Comma:        return "Space"; break;
	case KeyCode::Minus:        return "Space"; break;
	case KeyCode::Period:       return "Space"; break;
	case KeyCode::Slash:        return "Space"; break;

	case KeyCode::D0: return "D0"; break;
	case KeyCode::D1: return "D1"; break;
	case KeyCode::D2: return "D2"; break;
	case KeyCode::D3: return "D3"; break;
	case KeyCode::D4: return "D4"; break;
	case KeyCode::D5: return "D5"; break;
	case KeyCode::D6: return "D6"; break;
	case KeyCode::D7: return "D7"; break;
	case KeyCode::D8: return "D8"; break;
	case KeyCode::D9: return "D8"; break;

	case KeyCode::Semicolon: return "Semicolon"; break;
	case KeyCode::Equal:     return "Equal"; break;

	case KeyCode::A: return "A"; break;
	case KeyCode::B: return "B"; break;
	case KeyCode::C: return "C"; break;
	case KeyCode::D: return "D"; break;
	case KeyCode::E: return "E"; break;
	case KeyCode::F: return "F"; break;
	case KeyCode::G: return "G"; break;
	case KeyCode::H: return "H"; break;
	case KeyCode::I: return "I"; break;
	case KeyCode::J: return "J"; break;
	case KeyCode::K: return "K"; break;
	case KeyCode::L: return "L"; break;
	case KeyCode::M: return "M"; break;
	case KeyCode::N: return "N"; break;
	case KeyCode::O: return "O"; break;
	case KeyCode::P: return "P"; break;
	case KeyCode::Q: return "Q"; break;
	case KeyCode::R: return "R"; break;
	case KeyCode::S: return "S"; break;
	case KeyCode::T: return "T"; break;
	case KeyCode::U: return "U"; break;
	case KeyCode::V: return "V"; break;
	case KeyCode::W: return "W"; break;
	case KeyCode::X: return "X"; break;
	case KeyCode::Y: return "Y"; break;
	case KeyCode::Z: return "Z"; break;

	default: return std::to_string((uint16_t)key_code);
	}
}

std::string GetMouseCodeString(MouseCode mouse_code) {
	switch (mouse_code) {

	case MouseCode::Left:      return "Left"; break;
	case MouseCode::Right:     return "Right"; break;
	case MouseCode::Middle:    return "Middle"; break;
	case MouseCode::Button3:   return "Button3"; break;
	case MouseCode::Button4:   return "Button4"; break;
	case MouseCode::Button5:   return "Button5"; break;
	case MouseCode::Button6:   return "Button6"; break;
	case MouseCode::Last:      return "Last"; break;

	default: return std::to_string((uint16_t)mouse_code);
	}
}