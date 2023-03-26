#include <iostream>
#include <fstream>
#include <map>

class GameManager final
{
private:
	std::string _filepath;

public:

private:
	bool init();
	void load();
	
public:
	static GameManager* getInstance();
	
    template<typename T>
    T get(const std::string& key);

    template<typename T>
    void set(const std::string& key, const T& val);
	
	void save();
	void print();
};