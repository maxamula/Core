#pragma once
#include <windows.h>
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event {
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	public:
		Event() :type(Type::Invalid), code(0u)
		{}
		Event(Type type, BYTE code) : type(type), code(code)
		{}
		bool IsValid() const
		{
			return type != Type::Invalid;
		}
		bool IsPress() const
		{
			return type == Type::Press;
		}
		bool IsRelease() const
		{
			return type == Type::Release;
		}
		BYTE GetCode() const
		{
			return code;
		}
	private:
		Type type;
		BYTE code;
	};
public:
	Keyboard();
	bool KeyPressed(BYTE keycode) const;
	Event ReadKey();
	bool IsEmpty() const;
	void ClearKey();

	char ReadChar();
	bool IsEmptyCh() const;
	void ClearCh();

	void EnableAutorepeat();
	void DisableAutorepeat();
	bool Autorepeat() const;
private:
	void OnKeyPressed(BYTE keycode);
	void OnKeyReleased(BYTE keycode);
	void OnChar(char ch);
	void ClearState();
	template <typename T>
	static void TrimBuffer(std::queue<T>& buffer);
private:
	static constexpr UINT nKeys = 256u;
	static constexpr UINT bufferSize = 16u;
	bool bAutorepeat = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};

