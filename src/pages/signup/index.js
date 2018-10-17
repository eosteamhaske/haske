import React from "react";
import "./index.css";
import CardBar from "../../components/CardBar";
import { Button, Form } from 'semantic-ui-react'


export default () => (
  <div className="haske-container">
    <div className="card">
      <CardBar />
      <div className="sign-up row">
        <Form>
          <Form.Field>
            <label>Fullname</label>
            <input placeholder="e.g John Doe" />
          </Form.Field>
          <Form.Field>
            <label>Country</label>
            <input placeholder="e.g Nigeria" />
          </Form.Field>
          <Form.Field>
            <label>Mobile Number</label>
            <input placeholder="e.g 234812345690" />
          </Form.Field>
          <Button type="submit">Submit</Button>
        </Form>
      </div>
    </div>
  </div>
);
