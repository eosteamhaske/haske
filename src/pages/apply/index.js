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
            <label>Document Title</label>
            <input type='text' placeholder="e.g Passport" />
          </Form.Field>
          <Form.Field>
            <label>Upload Document</label>
            <input type='file' placeholder="e.g Upload document" />
          </Form.Field>
          <Form.Field>
            <label>Asset type</label>
            <input type='text' placeholder="e.g Asset" />
          </Form.Field>
          <Button type="submit">Apply</Button>
        </Form>
      </div>
    </div>
  </div>
);
