#pragma once
//Abstract class to make different Tests


namespace Test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}


		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

	};
}
