#pragma once

namespace TigerEngine {
using KeyCode = int;

namespace Key {
constexpr KeyCode Unknown = -1;

constexpr KeyCode Space = 32;
constexpr KeyCode Apostrophe = 39;
constexpr KeyCode Comma = 44;
constexpr KeyCode Minus = 45;
constexpr KeyCode Period = 46;
constexpr KeyCode Slash = 47;

constexpr KeyCode D0 = 48;
constexpr KeyCode D1 = 49;
constexpr KeyCode D2 = 50;
constexpr KeyCode D3 = 51;
constexpr KeyCode D4 = 52;
constexpr KeyCode D5 = 53;
constexpr KeyCode D6 = 54;
constexpr KeyCode D7 = 55;
constexpr KeyCode D8 = 56;
constexpr KeyCode D9 = 57;

constexpr KeyCode Semicolon = 59;
constexpr KeyCode Equal = 61;

constexpr KeyCode A = 65;
constexpr KeyCode B = 66;
constexpr KeyCode C = 67;
constexpr KeyCode D = 68;
constexpr KeyCode E = 69;
constexpr KeyCode F = 70;
constexpr KeyCode G = 71;
constexpr KeyCode H = 72;
constexpr KeyCode I = 73;
constexpr KeyCode J = 74;
constexpr KeyCode K = 75;
constexpr KeyCode L = 76;
constexpr KeyCode M = 77;
constexpr KeyCode N = 78;
constexpr KeyCode O = 79;
constexpr KeyCode P = 80;
constexpr KeyCode Q = 81;
constexpr KeyCode R = 82;
constexpr KeyCode S = 83;
constexpr KeyCode T = 84;
constexpr KeyCode U = 85;
constexpr KeyCode V = 86;
constexpr KeyCode W = 87;
constexpr KeyCode X = 88;
constexpr KeyCode Y = 89;
constexpr KeyCode Z = 90;

constexpr KeyCode LeftBracket = 91;
constexpr KeyCode Backslash = 92;
constexpr KeyCode RightBracket = 93;
constexpr KeyCode GraveAccent = 96;
constexpr KeyCode World1 = 161;
constexpr KeyCode World2 = 162;
constexpr KeyCode Escape = 256;
constexpr KeyCode Enter = 257;
constexpr KeyCode Tab = 258;
constexpr KeyCode Backspace = 259;
constexpr KeyCode Insert = 260;
constexpr KeyCode Delete = 261;
constexpr KeyCode Right = 262;
constexpr KeyCode Left = 263;
constexpr KeyCode Down = 264;
constexpr KeyCode Up = 265;
constexpr KeyCode PageUp = 266;
constexpr KeyCode PageDown = 267;
constexpr KeyCode Home = 268;
constexpr KeyCode End = 269;
constexpr KeyCode CapsLock = 280;
constexpr KeyCode ScrollLock = 281;
constexpr KeyCode NumLock = 282;
constexpr KeyCode PrintScreen = 283;
constexpr KeyCode Pause = 284;

constexpr KeyCode F1 = 290;
constexpr KeyCode F2 = 291;
constexpr KeyCode F3 = 292;
constexpr KeyCode F4 = 293;
constexpr KeyCode F5 = 294;
constexpr KeyCode F6 = 295;
constexpr KeyCode F7 = 296;
constexpr KeyCode F8 = 297;
constexpr KeyCode F9 = 298;
constexpr KeyCode F10 = 299;
constexpr KeyCode F11 = 300;
constexpr KeyCode F12 = 301;
constexpr KeyCode F13 = 302;
constexpr KeyCode F14 = 303;
constexpr KeyCode F15 = 304;
constexpr KeyCode F16 = 305;
constexpr KeyCode F17 = 306;
constexpr KeyCode F18 = 307;
constexpr KeyCode F19 = 308;
constexpr KeyCode F20 = 309;
constexpr KeyCode F21 = 310;
constexpr KeyCode F22 = 311;
constexpr KeyCode F23 = 312;
constexpr KeyCode F24 = 313;
constexpr KeyCode F25 = 314;

constexpr KeyCode KP0 = 320;
constexpr KeyCode KP1 = 321;
constexpr KeyCode KP2 = 322;
constexpr KeyCode KP3 = 323;
constexpr KeyCode KP4 = 324;
constexpr KeyCode KP5 = 325;
constexpr KeyCode KP6 = 326;
constexpr KeyCode KP7 = 327;
constexpr KeyCode KP8 = 328;
constexpr KeyCode KP9 = 329;
constexpr KeyCode KPDecimal = 330;
constexpr KeyCode KPDivide = 331;
constexpr KeyCode KPMultiply = 332;
constexpr KeyCode KPSubtract = 333;
constexpr KeyCode KPAdd = 334;
constexpr KeyCode KPEnter = 335;
constexpr KeyCode KPEqual = 336;

constexpr KeyCode LeftShift = 340;
constexpr KeyCode LeftControl = 341;
constexpr KeyCode LeftAlt = 342;
constexpr KeyCode LeftSuper = 343;
constexpr KeyCode RightShift = 344;
constexpr KeyCode RightControl = 345;
constexpr KeyCode RightAlt = 346;
constexpr KeyCode RightSuper = 347;

constexpr KeyCode Menu = 348;
}

namespace Mouse {
constexpr KeyCode Button1 = 0;
constexpr KeyCode Button2 = 1;
constexpr KeyCode Button3 = 2;
constexpr KeyCode Button4 = 3;
constexpr KeyCode Button5 = 4;
constexpr KeyCode Button6 = 5;
constexpr KeyCode Button7 = 6;
constexpr KeyCode Button8 = 7;

constexpr KeyCode ButtonLeft = Button1;
constexpr KeyCode ButtonRight = Button2;
constexpr KeyCode ButtonMiddle = Button3;
}
}
