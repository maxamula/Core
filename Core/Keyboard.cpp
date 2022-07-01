#include "Keyboard.h"

Keyboard::Keyboard()
{

}

bool Keyboard::KeyPressed(BYTE keycode) const
{
	return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey()
{
	if (keybuffer.size() > 0u)
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

bool Keyboard::IsEmpty() const
{
	return keybuffer.empty();
}

char Keyboard::ReadChar()
{
	if (charbuffer.size() > 0u)
	{
		BYTE charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::IsEmptyCh() const
{
	return charbuffer.empty();
}

void Keyboard::ClearKey()
{
	keybuffer = std::queue<Event>();
}

void Keyboard::ClearCh()
{
	charbuffer = std::queue<char>();
}

void Keyboard::ClearState()
{
	for (int i = 0; i < 256; i++)
	{
		keystates[i] = false;
	}
}

void Keyboard::EnableAutorepeat()
{
	bAutorepeat = true;
}

void Keyboard::DisableAutorepeat()
{
	bAutorepeat = false;
}

bool Keyboard::Autorepeat() const
{
	return bAutorepeat;
}

void Keyboard::OnKeyPressed(BYTE keycode)
{
	keystates[keycode] = true;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(BYTE keycode)
{
	keystates[keycode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char ch)
{
	charbuffer.push(ch);
	TrimBuffer(charbuffer);
}

template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}