void listAll(const MenuItem* m, string path) // two-parameter overload
{
    string new_path;
    if(m -> name() != "")
    {
        cout << path + m -> name() << endl;
        new_path = path + m->name() + "/";
    }
    
    if(m->menuItems() != NULL)
    {
        const CompoundMenuItem* comp_m = dynamic_cast< const CompoundMenuItem*>(m);
        int size  = (comp_m -> menuItems())->size();
        for(int j=0; j < size; j++)
        {
            listAll((*comp_m ->menuItems())[j], new_path);
        }
    }
}
