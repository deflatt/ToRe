export module App;

export import App.GUI;

export namespace App {

	export class App {

	public:

		void Init() {
			gui.Init();
		}

		bool IsRunning() {
			return gui.IsOpen();
		}

		void Update() {
			gui.Update();
		}

		GUI::GUI gui = {};

	};

}