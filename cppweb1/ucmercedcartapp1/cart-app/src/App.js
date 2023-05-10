import logo from "./images/seal.png";
import "./App.css";
import Header from "./components/header/header";
import React from "react";

function App() {
  const userRef = React.useRef(null);
  const passRef = React.useRef(null);

  const loginUserRef = React.useRef(null);
  const loginPassRef = React.useRef(null);

  const handleLogin = () => {
    const email = loginUserRef.current.value;
    const password = loginPassRef.current.value;
    if (!email || !password) {
      alert("Please enter email and password");
      return;
    }
    fetch("/login/", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ email: email, password: password }),
    })
      .then((res) => res.json())
      .then((json) => {
        console.log(json);
        sessionStorage.setItem("email", email);
        sessionStorage.setItem("loginResponse", JSON.stringify(json));
      });
  };

  const handleRegister = () => {
    const email = userRef.current.value;
    const password = passRef.current.value;
    if (!email || !password) {
      alert("Please enter email and password");
      return;
    }
    fetch("/register/", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ email: email, password: password }),
    })
      .then((res) => res.json())
      .then((json) => {
        console.log(json);
        sessionStorage.setItem("email", email);
        sessionStorage.setItem("registerResponse", JSON.stringify(json));
      })
      .catch((err) => {
        console.log(err);
      });
  };

  const handleLoadCartData = React.useCallback(() => {
    fetch("/check_out/", {
      method: "GET",
      headers: {
        "Content-Type": "application/json",
      },
    })
      .then((res) => res.json())
      .then((json) => {
        console.log(json);
      });
  });

  React.useEffect(() => {
    handleLoadCartData();
  }, []);

  return (
    <>
      <Header />
      <div className="App">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <p>
            Edit <code>src/App.js</code> and save to reload
          </p>
          <a
            className="App-link"
            href="https://reactjs.org"
            target="_blank"
            rel="noopener noreferrer"
          >
            Learn React
          </a>
        </header>
        <div>
          <input type="text" ref={userRef} placeholder="email" />
          <input type="password" ref={passRef} placeholder="password" />
          <button onClick={handleRegister}>Register</button>
        </div>

        <div>
          <input type="text" ref={loginUserRef} placeholder="email" />
          <input type="password" ref={loginPassRef} placeholder="password" />
          <button onClick={handleLogin}>Login</button>
        </div>
      </div>
    </>
  );
}

export default App;
