#pragma once

#include "FVector2.h"
#include "intdef.h"
#include <memory>

/// <summary> 
/// Repersents an keyboard key
/// </summary>
struct Key
{
	/// <summary> 
	/// was key down?
	/// </summary>
	bool wasDown;

	/// <summary> 
	/// is key currently down
	/// </summary>
	bool isDown;

	/// <summary> 
	/// is key currently released
	/// </summary>
	bool isUp;

	/// <summary> 
	/// the raw code of this key
	/// </summary>
	char key;
};

class AppInput
{
	public:

	AppInput();
	~AppInput();

	private:

	Key keys[256];

	public:

	/// <summary> 
	/// Initializes input
	/// </summary>
	void     Init();

	/// <summary> 
	/// Reads input
	/// </summary>
	void     Read();

	/// <summary> 
	/// Get current mouse position
	/// </summary>
	FVector2 GetMousePosition() const;

	/// <summary> 
	/// Check if key is currently down
	/// </summary>
	bool     GetKeyDown(UInt inKey);

	/// <summary> 
	/// Check if key is currently up
	/// </summary>
	bool     GetKeyUp(UInt inKey);

	/// <summary> 
	/// Check if key is currently hold by user
	/// </summary>
	bool     GetKey(UInt key);

	/// <summary> 
	/// Check if mouse key is just pressed
	/// </summary>
	bool     GetMouseButtonDown(UInt inButton);

	/// <summary> 
	/// Check if mouse key is just released
	/// </summary>
	bool     GetMouseButtonUp(UInt inButton);

	/// <summary> 
	/// Check if mouse key is currently hold by user
	/// </summary>
	bool     GetMouseButton(UInt inButton);

	private:

	friend class App;
	friend struct std::default_delete<AppInput>;
	friend class std::allocator<AppInput>;
};