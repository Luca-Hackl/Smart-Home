import React from 'react';
import {checkLoginState} from "./TilesCust";
import Register from "./Register";

export default class Login extends React.Component{
    constructor(props) {
        super(props);
        this.state={
            email:'',
            password:'',
            register: false
        };

        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
        this.handler = this.handler.bind(this)
    }

    handleChange(event) {
        const value = event.target.value;
        this.setState({
            [event.target.name]: value,
        });
  }
  async handleSubmit(event) {
      event.preventDefault()
      await checkLogin(this.state.email, this.state.password)
      const test = await checkLoginState()
      if (test[0] === true){
          this.props.handler()
      }
  }

  handler(){
        this.setState({register:false})
  }
render() {

    if (!this.state.register) {
        return (
            <section className="h-screen">
                <div className="px-6 h-full text-gray-800">
                    <div
                        className="flex xl:justify-center lg:justify-between justify-center items-center flex-wrap h-full g-6">
                        <div
                            className="grow-0 shrink-1 md:shrink-0 basis-auto xl:w-6/12 lg:w-6/12 md:w-9/12 mb-12 md:mb-0">
                            <img src="https://mdbcdn.b-cdn.net/img/Photos/new-templates/bootstrap-login-form/draw2.webp"
                                 className="w-full" alt="Sample"/>
                        </div>
                        <div className="xl:ml-20 xl:w-5/12 lg:w-5/12 md:w-8/12 mb-12 md:mb-0">
                            <form onSubmit={this.handleSubmit}>
                                <div className="mb-6">
                                    <input
                                        name="email"
                                        className="form-control block w-full px-4 py-2 text-xl font-normal text-gray-700 bg-white bg-clip-padding border border-solid border-gray-300 rounded transition ease-in-out m-0 focus:text-gray-700 focus:bg-white focus:border-blue-600 focus:outline-none"
                                        id="FormControlInput1"
                                        placeholder="Email address"
                                        value={this.state.email}
                                        onChange={this.handleChange}
                                    />
                                </div>
                                <div className="mb-6">
                                    <input
                                        name="password"
                                        className="form-control block w-full px-4 py-2 text-xl font-normal text-gray-700 bg-white bg-clip-padding border border-solid border-gray-300 rounded transition ease-in-out m-0 focus:text-gray-700 focus:bg-white focus:border-blue-600 focus:outline-none"
                                        id="FormControlInput2"
                                        placeholder="Password"
                                        value={this.state.password}
                                        onChange={this.handleChange}
                                    />
                                </div>
                                <div className="text-center lg:text-left">
                                    <input
                                        type="submit" value="Login"
                                        className="inline-block px-7 py-3 bg-blue-600 text-white font-medium text-sm leading-snug uppercase rounded shadow-md hover:bg-blue-700 hover:shadow-lg focus:bg-blue-700 focus:shadow-lg focus:outline-none focus:ring-0 active:bg-blue-800 active:shadow-lg transition duration-150 ease-in-out"
                                    >
                                    </input>
                                    <p className="text-sm font-semi-bold mt-2 pt-1 mb-0">
                                        Don't have an account?
                                        <a
                                           className="text-red-600 hover:text-red-700 focus:text-red-700 transition duration-200 ease-in-out" onClick={()=>this.setState({register:true})}> Register</a>
                                    </p>
                                </div>
                            </form>
                        </div>
                    </div>
                </div>
            </section>
        )
    } else {
        return(
        <Register handler={()=>this.handler()}></Register>
        )
    }
}
}

export async function checkLogin(email, password) {
    await fetch("/api/login", {
            method: "POST",
            cache: "no-cache",
            headers: {
                "content_type": "application/json",
            },
            body: JSON.stringify({[email]:password})
        }
    )
}