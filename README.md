# myDB 
Find minimal cover and keys from given FDs

# ex
    A B C ->D 
    C D E ->F 
    D E ->A 
    D E ->B 
    C F ->B 
    B D F ->E 
    A E ->C 
    B C ->F 
    A B ->E 
    A B ->F 
    B F ->C 
    
    minimal cover
    D E ->A F 
    C F ->B 
    B D F ->E 
    A E ->C 
    B C ->F 
    A B ->D F 
    B F ->C 
    
    keys
    D E 
    C D F 
    A E F 
    B C D 
    A B 
    B D F 
