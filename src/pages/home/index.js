import React from "react";
import "./index.css";
import Button from "../../components/ButtonWithRouter";
import CardBar from "../../components/CardBar";
import api from '../../api'

export default () => (
  <div className="haske-container">
    <div className="card mobile">
      <CardBar />
      <div className="home row ">
        <Button to="/register" btnText="Sign up" />{" "}
        <Button to="/apply" btnText="Apply For Loan" />
      </div>
    </div>
  </div>
);
