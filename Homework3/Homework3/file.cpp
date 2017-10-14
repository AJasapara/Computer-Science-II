#include <iostream>
#include <string>
using namespace std;

class File {
public:
	File(string n) {
		m_name = n;
	}
	virtual string name() const {
		return m_name;
	}
	virtual void open() const = 0;
	virtual void redisplay() const {
		cout << "refresh the screen";
	}
	virtual ~File() {
		cout << endl;
	}
private:
	string m_name;
};

class TextMsg : public File {
public:
	TextMsg(string n): File(n){}
	virtual void open() const {
		cout << "open text message";
	}
	virtual ~TextMsg() {
		cout << "Destroying " << File::name() << ", a text message";
	}
};

class Video : public File {
public:
	Video(string n, int run) : File(n), runTime(run) {}
	virtual void open() const {
		cout << "play " << runTime << " second video";
	}
	virtual void redisplay() const {
		cout << "replay video";
	}
	virtual ~Video() {
		cout << "Destroying " << File::name() << ", a video";
	}
private:
	int runTime;
};

class Picture : public File {
public:
	Picture(string n) : File(n) {}
	virtual void open() const {
		cout << "show picture";
	}
	virtual ~Picture() {
		cout << "Destroying the picture " << File::name();
	}
};

void openAndRedisplay(const File* f)
{
	cout << f->name() << ": ";
	f->open();
	cout << endl << "Redisplay: ";
	f->redisplay();
	cout << endl;
}

int main()
{
	File* files[4];
	files[0] = new TextMsg("fromFred.txt");
	// Videos have a name and running time
	files[1] = new Video("goblin.mpg", 3780);
	files[2] = new Picture("kitten.jpg");
	files[3] = new Picture("baby.jpg");
	for (int k = 0; k < 4; k++)
		openAndRedisplay(files[k]);

	// Clean up the files before exiting
	cout << "Cleaning up." << endl;
	for (int k = 0; k < 4; k++)
		delete files[k];
}