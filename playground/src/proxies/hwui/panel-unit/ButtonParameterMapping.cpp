#include <proxies/hwui/panel-unit/ButtonParameterMapping.h>

ButtonParameterMapping::ButtonParameterMapping()
{
  addMapping(0, { 0, 15, 294 });
  addMapping(1, { 38, 49, 296 });
  addMapping(2, { 46, 50 });
  addMapping(3, { 19, 34, 295 });
  addMapping(4, { 2 });
  addMapping(5, { 40, 42 });
  addMapping(6, { 48 });
  addMapping(7, { 21 });
  addMapping(8, { 4 });
  addMapping(9, { 44, 297 });
  addMapping(10, { 51, 52 });
  addMapping(11, { 23 });
  addMapping(12, { 6 });
  addMapping(13, { 14 });
  addMapping(14, { 33 });
  addMapping(15, { 25 });
  addMapping(16, { 8 });
  addMapping(17, { 17, 18 });
  addMapping(18, { 36, 37 });
  addMapping(19, { 27 });
  addMapping(20, { 10, 16 });
  addMapping(21, { 12 });
  addMapping(22, { 31 });
  addMapping(23, { 29, 35 });
  addMapping(24, { 53, 56, 55 });
  addMapping(25, { 71, 73 });
  addMapping(26, { 101, 103 });
  addMapping(27, { 83, 86, 85 });
  addMapping(28, { 57, 59 });
  addMapping(29, { 74 });
  addMapping(30, { 104 });
  addMapping(31, { 87, 89 });
  addMapping(32, { 301 });
  addMapping(33, { 75 });
  addMapping(34, { 105 });
  addMapping(35, { 302 });
  addMapping(36, { 60, 62, 63 });
  addMapping(37, { 76 });
  addMapping(38, { 106 });
  addMapping(39, { 90, 92, 93 });
  addMapping(40, { 64, 66, 67 });
  addMapping(41, { 78, 80 });
  addMapping(42, { 108, 110 });
  addMapping(43, { 94, 96, 97 });
  addMapping(44, { 68, 70, 303 });
  addMapping(45, { 81 });
  addMapping(46, { 111 });
  addMapping(47, { 98, 100, 304 });
  addMapping(48, { 156 });
  addMapping(49, { 113 });
  addMapping(50, { 136 });
  addMapping(51, { 169, 171 });
  addMapping(52, { 158 });
  addMapping(53, { 115, 118, 117 });
  addMapping(54, { 138 });
  addMapping(55, { 172, 174 });
  addMapping(56, { 160 });
  addMapping(57, { 119, 305, 121 });
  addMapping(58, { 140, 143, 142 });
  addMapping(59, { 175, 177 });
  addMapping(60, { 162 });
  addMapping(61, { 123, 126, 125, 127 });
  addMapping(62, { 144, 147, 146 });
  addMapping(63, { 178, 180 });
  addMapping(64, { 164, 166, 167 });
  addMapping(65, { 129, 132, 131 });
  addMapping(66, { 148, 150, 152 });
  addMapping(67, { 181, 183, 184 });
  addMapping(68, { 299, 168 });
  addMapping(69, { 133, 135 });
  addMapping(70, { 153, 155 });
  addMapping(71, { 185, 187 });

  addMapping(72, { 214 });
  addMapping(73, { 188, 190, 191 });

  addMapping(74, { 225, 227 });
  addMapping(75, { 243 });

  addMapping(76, { 307, 213 });
  addMapping(77, { 192, 194, 196 });

  addMapping(78, { 229, 231, 232 });
  addMapping(79, { 244 });

  addMapping(80, { 211, 216, 218 });
  addMapping(81, { 199, 197 });

  addMapping(82, { 233 });
  addMapping(83, { 245 });

  addMapping(84, { 308, 310 });
  addMapping(85, { 201, 203 });

  addMapping(86, { 235, 237 });
  addMapping(87, { 246 });

  addMapping(88, { 219, 221, 222 });
  addMapping(89, { 204, 207, 206 });

  addMapping(90, { 238, 240 });
  addMapping(91, { 249, 250, 252, 253 });

  addMapping(92, { 223 });
  addMapping(93, { 209 });

  addMapping(94, { 241 });
  addMapping(95, { 247, 248, 312 });
}

ButtonParameterMapping::~ButtonParameterMapping()
{
}

void ButtonParameterMapping::addMapping(int buttonID, std::initializer_list<int> parameterIDs)
{
  for(int paramID : parameterIDs)
  {
    g_assert(m_paramIDToButtonID.find(paramID) == m_paramIDToButtonID.end());
    m_paramIDToButtonID[paramID] = buttonID;
  }

  g_assert(m_buttonIDToParam.find(buttonID) == m_buttonIDToParam.end());
  m_buttonIDToParam[buttonID] = std::list<int>(parameterIDs);
}

void ButtonParameterMapping::forEachButton(std::function<void(int, const std::list<int> &)> cb)
{
  for(auto &a : m_buttonIDToParam)
    cb(a.first, a.second);
}

int ButtonParameterMapping::findButton(int param) const
{
  auto it = m_paramIDToButtonID.find(param);

  if(it != m_paramIDToButtonID.end())
    return it->second;

  return -1;
}

std::list<int> ButtonParameterMapping::findParameters(int button) const
{
  auto it = m_buttonIDToParam.find(button);

  if(it != m_buttonIDToParam.end())
    return it->second;

  return std::list<int>();
}
