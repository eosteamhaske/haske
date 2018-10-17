import React from "react";
import "./index.css";
import Button from "../../components/ButtonWithRouter";

export default () => (
  <div className="landing-page">
      <div className='lp-nav-bar'><Button btnText='BORROW' to='/dashboard'/><Button btnText='LEND' to='/dashboard' /></div>
      <div className='lp-text'><span>HASKE</span><span>- Tokenised Loan on EOS Blockchain</span></div>
  </div>
);
