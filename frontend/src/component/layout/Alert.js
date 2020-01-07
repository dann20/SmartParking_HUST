import React, { Component, Fragment } from 'react';
import { withAlert } from 'react-alert';
import { connect } from 'react-redux';
import PropTypes from 'prop-types';
export class Alert extends Component {
    static propTypes = {
        error: PropTypes.object.isRequired,
        message: PropTypes.object.isRequired
    }

    componentDidUpdate(prevProps) {
        const {  alert, message } = this.props;
       
        if (message !== prevProps.message) {
            if (message.addReceipt) alert.success(message.addReceipt);
            if (message.passwordNotMatch) alert.error(message.passwordNotMatch);
        }
    }

    render() {
        return <Fragment />
    }
}

const mapStateToProps = state => ({
    error: state.errors,
    message: state.messages
});

export default connect(mapStateToProps)(withAlert()(Alert));