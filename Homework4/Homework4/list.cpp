void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m == NULL)
		return;
	string str = path + m->name();
	if (m->name() != "") {
		cout << str << endl;
		str += "/";
	}
	if (m->menuItems() == NULL)
		return;
	for (vector<MenuItem*>::const_iterator it = m->menuItems()->begin(); it != m->menuItems()->end(); it++)
		listAll(*it, str);
}