import React, { Component, Fragment } from 'react';
import { connect } from 'react-redux';
import PropTypes from 'prop-types';
import { getReceipts, addReceipt } from '../../actions/receipts';

export class Receipts extends Component {
    state = {
        lot_no: "",
        enter_time: "",
        exit_time: "",
        fee: "",
    };
    static propTypes = {
        receipts: PropTypes.array.isRequired,
        getReceipts: PropTypes.func.isRequired,
        addReceipt: PropTypes.func.isRequired
    }

    componentDidMount() {
        this.props.getReceipts();
          
    }
   

    render() {
        return (
            <Fragment>
                <h2>Receipts</h2>
                <table className="table table-striped">
                    <thead>
                        <tr>
                            <th>Lot_No</th>
                            <th>Enter</th>
                            <th>Exit</th>
                            <th>Fee (VND)</th>
                            <th />
                        </tr>
                    </thead>
                    <tbody>
                        {this.props.receipts.map(receipt => (
                            <tr key={receipt.id}>
                                
                                <td>{receipt.lot_no}</td>
                                <td>{receipt.enter_time}</td>
                                <td>{receipt.exit_time}</td>
                                <td>{receipt.fee}</td>
                               
                            </tr>
                        ))}
                    </tbody>
                </table>
                
            </Fragment>
        )
    }
}

const mapStateToProps = state => ({
    receipts: state.receipts.receipts
});

export default connect(mapStateToProps, { getReceipts, addReceipt })(Receipts);