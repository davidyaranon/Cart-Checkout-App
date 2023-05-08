import React from 'react';

const Header = () => {

  React.useEffect(() => {
    document.title = "UC Merced Cart App";
  })

    return (
        <header>
            <h1>UC Merced Cart App</h1>
        </header>
    );

};

export default Header;