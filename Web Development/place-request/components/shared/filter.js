'use client'

import {
    Button,
    Card,
    CardBody,
    CardFooter,

} from '@chakra-ui/react';

export default function Filter({ children, setIsSubmit }) {
    const handleSubmit = async () => {
        setIsSubmit(true);
    };

    return (
        <>
            <Card align='center' h='full'>
                <CardBody>
                    {children}
                </CardBody>
                <CardFooter>
                    <Button colorScheme='telegram' size='md' onClick={() => handleSubmit()}>筛选</Button>
                </CardFooter>
            </Card >

        </>
    );
}   