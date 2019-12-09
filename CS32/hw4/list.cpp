void listAll(const MenuItem* m, string path) // two-parameter overload
{
    const vector<MenuItem*>* v = m->menuItems();
    if(v == NULL) return;
    for(int i = 0; i < v->size(); i++)
    {
        string spath = path;
        if(path != "")
        {
            spath+="/";
        }
        spath += (*v)[i]->name();
        cout << spath << endl;
        listAll((*v)[i], spath);  
    }
}
