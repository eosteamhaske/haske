import React from "react";
import "./index.css";
import CardBar from "../../components/CardBar";
import { Icon, Table } from "semantic-ui-react";
import Button from "../../components/ButtonWithRouter";

export default () => (
  <div className="haske-container">
    <div className="card">
      <CardBar />
      <div className="sign-up row">
        <Table celled>
          <Table.Header>
            <Table.Row>
              <Table.HeaderCell>Name</Table.HeaderCell>
              <Table.HeaderCell>Status</Table.HeaderCell>
              <Table.HeaderCell>Notes</Table.HeaderCell>
            </Table.Row>
          </Table.Header>

          <Table.Body>
            <Table.Row>
              <Table.Cell>No Name Specified</Table.Cell>
              <Table.Cell>Approved</Table.Cell>
              <Table.Cell>None</Table.Cell>
            </Table.Row>
            <Table.Row error>
              <Table.Cell>Jimmy</Table.Cell>
              <Table.Cell>Cannot pull data</Table.Cell>
              <Table.Cell>None</Table.Cell>
            </Table.Row>
            <Table.Row>
              <Table.Cell>Jamie</Table.Cell>
              <Table.Cell>Approved</Table.Cell>
              <Table.Cell error>
                <Icon name="attention" />
                Classified
              </Table.Cell>
            </Table.Row>
            <Table.Row>
              <Table.Cell>Jill</Table.Cell>
              <Table.Cell>Approved</Table.Cell>
              <Table.Cell>None</Table.Cell>
            </Table.Row>
          </Table.Body>
        </Table>
        <Button to="loan-history" btnText="Loan History" />
      </div>
    </div>
  </div>
);
